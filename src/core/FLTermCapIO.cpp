/*  Copyright 2017 Gibbon aka 'atsb'
    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include <stdarg.h>
#include <time.h>
#include <ctype.h>
#include <sys/stat.h>
#include <setjmp.h>
#include <fcntl.h>
#include <string>

#if defined WINDOWS || WINDOWS_VS
	#include <io.h>
	#include <conio.h>
#endif

#if defined NIX || NIX_LOCAL
	#include <unistd.h>
	#include <sys/ioctl.h>
	#ifndef FIONREAD
		#include <sys/socket.h>
	#endif
#endif


#include "../terminal/FLTerminal.hpp"
#include "FLFuncs.hpp"
#include "FLTermCapIO.hpp"
#include "../dungeon/FLDungeon.hpp"
#include "FLScores.hpp"
#include "../termcap/FLTermcap.hpp"
#include "../config/FLIncludeData.hpp"
#include "../templates/FLTMath.hpp"
#include "../terminal/FLGetChar.hpp"
#include "../terminal/FLOut.hpp"

using namespace std;

int lfd;			/*  output file numbers     */
int fd;				/*  input file numbers      */

static int ipoint = FL_INPUT_BUFFER, iepoint = FL_INPUT_BUFFER;
static char fl_retrieve_word_buffer[LINBUFSIZE];
static void ttputch(int);
static void tputs(const char *);
static void flush_buf(void);

/*
    Subroutine to set up terminal in correct mode for game

    Attributes off, clear screen, set scrolling region, set tty mode
*/
void
fl_setup_terminal(void) {
	fl_clear_and_reset_screen();
	enable_scroll = 1;
	get_final_char_keyboard = ansiterm_getch;
}

/*
    Subroutine to clean up terminal when the game is over

    Attributes off, clear screen, unset scrolling region, restore tty mode
*/
void
fl_clear_terminal(void) {
	ansiterm_clean_up();
	enable_scroll = 0;
	get_final_char_keyboard = ansiterm_getche;
}



/*
    ttgetch()       Routine to read in one character from the terminal
*/
char
ttgetch(void) {
	char byt;
	cdesc[FL_BYTES_IN]++;
	fl_output_buffer_flush();			/* be sure output buffer is flushed */
	byt = (char)(*get_final_char_keyboard)();
	if (byt == '\r') {
		byt = '\n';
	}
	return byt;
}

/*
    lprintf(format,args . . .)      printf to the output buffer
       char *format;
       ??? args . . .

    Enter with the format string in "format", as per printf() usage
       and any needed arguments following it
    Note: lprintf() only supports %s, %c and %d, with width modifier and left
       or right justification.
    No correct checking for output buffer overflow is done, but flushes
       are done beforehand if needed.
    Returns nothing of value.
*/
void
lprintf(const char *fmt, ...) {
	va_list vl;
	char buffer[FL_STRING_BUFFER_SIZE];
	const char *p;
	va_start(vl, fmt);
	vsprintf(buffer, fmt, vl);
	va_end(vl);
	p = buffer;
	while (*p != '\0') {
		fl_output_single_byte_to_output_buffer(*p);
		++p;
	}
}

/* This function will append the messages to a messagefile. ~Gibbon */
void
fl_display_message(const char *fmt, ...) {
	ofstream message_file;
	message_file.open(FL_MESSAGE_LOG_NAME, ios::app);
	va_list vl;
	char fl_message_buffer[FL_STRING_BUFFER_SIZE];
	const char *print_char;
	va_start(vl, fmt);
	vsprintf(fl_message_buffer, fmt, vl);
	va_end(vl);
	print_char = fl_message_buffer;
	/* Convert to std::string. ~Gibbon */
	std::string cppstr2 = fl_message_buffer;
	message_file << "Cave Level:" << levelname[level] << "\n" << cppstr2 << endl;
	message_file.close();
	while (*print_char != '\0') {
		fl_output_single_byte_to_output_buffer(*print_char);
		++print_char;
	}
}

/*
    lprint(int-integer)                send binary integer to output buffer
       int integer;

       +---------+---------+---------+---------+
       |   high  |         |         |   low   |
       |  order  |         |         |  order  |
       |   byte  |         |         |   byte  |
       +---------+---------+---------+---------+
      31  ---  24 23 --- 16 15 ---  8 7  ---   0

    The save order is low order first, to high order (4 bytes total)
       and is written to be system independent.
    No checking for output buffer overflow is done, but flushes if needed!
    Returns nothing of value.
*/
void
lprint(int x) {
	if (fl_buffer_pointer >= fl_buffer_pending) {
		fl_output_buffer_flush();
	}
	*fl_buffer_pointer++ = 255 & x;
	*fl_buffer_pointer++ = 255 & (x >> 8);
	*fl_buffer_pointer++ = 255 & (x >> 16);
	*fl_buffer_pointer++ = 255 & (x >> 24);
}

void
fl_output_single_byte_to_output_buffer(char ch) {
	*fl_buffer_pointer++ = ch;
	if (fl_buffer_pointer >= fl_buffer_pending) {
		fl_output_buffer_flush();
	}
	fl_output_buffer_flush();
}

void
fl_write_current_buffer_to_output_buffer(char *buf, int len) {
	char *str;
	int num2;
	if (len > 399) {	/* don't copy data if can just write it */
		cdesc[FL_BYTES_OUT] += len;
		for (str = buf; len > 0; --len) {
			fl_output_single_byte_to_output_buffer(*str++);
		}
	} else
		while (len) {
			if (fl_buffer_pointer >= fl_buffer_pending) {
				fl_output_buffer_flush();   /* if buffer is full flush it   */
			}
			num2 = lpbuf + FL_OUTPUT_BUFFER -
			       fl_buffer_pointer;	/*  # bytes left in output buffer   */
			if (num2 > len) {
				num2 = len;
			}
			str = fl_buffer_pointer;
			len -= num2;
			while (num2--) {
				*str++ = *buf++;  /* copy in the bytes */
			}
			fl_buffer_pointer = str;
		}
}

char
fl_read_char_from_input_buffer(void) {
	/*
	    Static ints please!

	    ~Gibbon
	*/
	static int i;
	if (ipoint != iepoint) {
		return (inbuffer[ipoint++]);
	}
	if (iepoint != FL_INPUT_BUFFER) {
		return (0);
	}
	#if defined WINDOWS
	if ((i = _read(fd, inbuffer, FL_INPUT_BUFFER)) <= 0)
	#endif
	#if defined NIX || NIX_LOCAL
		if ((i = read(fd, inbuffer, FL_INPUT_BUFFER)) <= 0)
	#endif
		{
			if (i != 0) {
				fprintf(stderr, "error reading from input file\n");
			}
			iepoint = ipoint = 0;
			return (0);
		}
	ipoint = 1;
	iepoint = i;
	return (*inbuffer);
}

/*
    int larint()            Read one integer from input buffer

       +---------+---------+---------+---------+
       |   high  |         |         |   low   |
       |  order  |         |         |  order  |
       |   byte  |         |         |   byte  |
       +---------+---------+---------+---------+
      31  ---  24 23 --- 16 15 ---  8 7  ---   0

    The save order is low order first, to high order (4 bytes total)
    Returns the int read
*/
int
larint(void) {
	int i;
	i = 255 & fl_read_char_from_input_buffer();
	i |= (255 & fl_read_char_from_input_buffer()) << 8;
	i |= (255 & fl_read_char_from_input_buffer()) << 16;
	i |= (255 & fl_read_char_from_input_buffer()) << 24;
	return (i);
}



/*
    lrfill(address,number)          put input bytes into a buffer
       char *address;
       int number;

    Reads "number" bytes into the buffer pointed to by "address".
    Returns nothing of value
*/
void
lrfill(char *adr, int num) {
	char *pnt;
	int num2;
	while (num) {
		if (iepoint == ipoint) {
			if (num > 5) {	/* fast way */
				#if defined WINDOWS
				if (_read(fd, adr, num) != num)
				#endif
				#if defined NIX || NIX_LOCAL
					if (read(fd, adr, num) != num)
				#endif
						fprintf(stderr, "error reading from input file\n");
				num = 0;
			} else {
				*adr++ = fl_read_char_from_input_buffer();
				--num;
			}
		} else {
			num2 = iepoint -
			       ipoint;	/*  # of bytes left in the buffer   */
			if (num2 > num) {
				num2 = num;
			}
			pnt = inbuffer + ipoint;
			num -= num2;
			ipoint += num2;
			while (num2--) {
				*adr++ = *pnt++;
			}
		}
	}
}



/*
    char *lgetw()           Get a whitespace ended word from input

    Returns pointer to a buffer that contains word.  If EOF, returns a NULL
*/
char *
lgetw(void) {
	char *lgp, cc;
	int n = LINBUFSIZE, quote = 0;
	lgp = fl_retrieve_word_buffer;
	do {
		cc = fl_read_char_from_input_buffer();
	} while ((cc <= 32) && (cc > '\0'));	/* eat whitespace */
	for (;; --n, cc = fl_read_char_from_input_buffer()) {
		if ((cc == '\0') && (lgp == fl_retrieve_word_buffer)) {
			return (NULL);    /* EOF */
		}
		if ((n <= 1) || ((cc <= 32) && (quote == 0))) {
			*lgp = '\0';
			return fl_retrieve_word_buffer;
		}
		if (cc != '"') {
			*lgp++ = cc;
		} else {
			quote ^= 1;
		}
	}
}

/*
    char *lgetl()       Function to read in a line ended by newline or EOF

    Returns pointer to a buffer that contains the line.  If EOF, returns NULL
*/
char *
lgetl(void) {
	int i = LINBUFSIZE;
	char ch;
	char *str = fl_retrieve_word_buffer;
	for (;; --i) {
		*str++ = ch = fl_read_char_from_input_buffer();
		if (ch == 0) {
			if (str == fl_retrieve_word_buffer + 1) {
				return (NULL);    /* EOF */
			}
			if ((ch == '\n') || (i <= 1)) {
				*str = 0;
				return (fl_retrieve_word_buffer);	/* line ended by EOF */
			}
		}
	}
}

/*  Just for the record, I removed all those _setmodes for *nix
    systems a while ago.  No need to declare these
    things to make it work on them,
    these systems just don't require it. ~Gibbon
*/

/*
    fl_create_new_file(filename)            Create a new file for write
       char *filename;

    fl_create_new_file((char*)0); means to the terminal
    Returns -1 if error, otherwise the file descriptor opened.

    Modernised this function and made it cleaner. ~Gibbon
*/
int
fl_create_new_file(char *str) {
	fl_buffer_pointer = lpbuf;
	fl_buffer_pending = lpbuf + FL_OUTPUT_BUFFER;
	if (str == NULL) {
		return (lfd = 1);
	}
	#if defined WINDOWS
	if ((lfd = _creat(str, _S_IWRITE)) < 0)
	#endif
	#if defined NIX || NIX_LOCAL
		if ((lfd = open(str, O_RDWR | O_CREAT, 0666)) < 0)
	#endif
		{
			lfd = 1;
			lprintf("error creating file <%s>\n", str);
			fl_output_buffer_flush();
			return (-1);
		}
	#if defined WINDOWS
	_setmode(lfd, O_BINARY);
	#endif
	return lfd;
}

/*
    fl_open_file(filename)         Open a file for read
       char *filename;

    fl_open_file(0) means from the terminal
    Returns -1 if error, otherwise the file descriptor opened.
*/
int
fl_open_file(char *str) {
	ipoint = iepoint = FL_INPUT_BUFFER;
	if (str == NULL) {
		return (fd = 0);
	}
	#if defined WINDOWS
	if ((fd = _open(str, 0)) < 0)
	#endif
	#if defined NIX || NIX_LOCAL
		if ((fd = open(str, 0)) < 0)
	#endif
		{
			fl_close_file_flush();
			lfd = 1;
			fl_buffer_pointer = lpbuf;
			return (-1);
		}
	#if defined WINDOWS
	_setmode(fd, O_BINARY);
	#endif
	return fd;
}

/*
    fl_append_to_file(filename)       Open for append to an existing file
       char *filename;

    fl_append_to_file(0) means to the terminal
    Returns -1 if error, otherwise the file descriptor opened.
*/
int
fl_append_to_file(char *str) {
	fl_buffer_pointer = lpbuf;
	fl_buffer_pending = lpbuf + FL_OUTPUT_BUFFER;
	if (str == NULL) {
		return (lfd = 1);
	}
	#if defined WINDOWS
	if ((lfd = _open(str, 2)) < 0)
	#endif
	#if defined NIX || NIX_LOCAL
		if ((lfd = open(str, 2)) < 0)
	#endif
		{
			lfd = 1;
			return (-1);
		}
	#if defined WINDOWS
	_setmode(lfd, O_BINARY);
	_lseek(lfd, 0L, 2);		/* seek to end of file */
	#endif
	#if defined NIX || NIX_LOCAL
	lseek(lfd, 0L, 2);		/* seek to end of file */
	#endif
	return lfd;
}

/*
    fl_close_the_file()                       close the input file

    Returns nothing of value.
*/
void
fl_close_the_file(void) {
	if (fd > 0) {
		#if defined WINDOWS
		_close(fd);
		#endif
		#if defined NIX || NIX_LOCAL
		close(fd);
		#endif
	}
}

void
fl_close_file_flush(void) {
	fl_output_buffer_flush();
	if (lfd > 2) {
		#if defined WINDOWS
		_close(lfd);
		#endif
		#if defined NIX || NIX_LOCAL
		close(lfd);
		#endif
	}
}

void
fl_termcap_cursor_position(int x, int y) {
	if (fl_buffer_pointer >= fl_buffer_pending) {
		fl_output_buffer_flush();
	}
	*fl_buffer_pointer++ = FL_CURSOR;
	*fl_buffer_pointer++ = static_cast<char>(x);
	*fl_buffer_pointer++ = static_cast<char>(y);
}

/* translated output buffer */
static char *outbuf = NULL;

/*
    ANSI control sequences.
    Modified and changed to const.
    ~Gibbon
*/

static const char TT_CLEAR_TO_END_OF_LINE[] = { 27, '[', 'K', 0 };
static char *TT_CLEAR_TO_END_OF_DISPLAY = NULL;
static const char TT_CLEAR_THE_SCREEN[] = { 27, '[', ';', 'H', 27, '[', '2', 'J', 0 };
static const char TT_CURSOR_MOVE[] = { 27, '[', '%', 'i', '%', '2', ';', '%', '2', 'H', 0 };
static const char TT_BEGIN_STANDARD_OUT_MODE[] = { 27, '[', '1', 'm', 0 };
static const char TT_STOP_STANDARD_OUT_MODE[] = { 27, '[', 'm', 0 };
static const char TT_TERMINAL_INITIALIZATION[] = { 27, '[', 'm', 0 };
static const char TT_TERMINAL_INITIALIZATION_END[] = { 27, '[', 'm', 0 };

void
fl_initialize_termcap_terminal_handling(void) {
	/* get memory for decoded output buffer */
	outbuf = (char*) operator new (FL_OUTPUT_BUFFER + 16);
	if (outbuf == NULL) {
		fprintf(stderr,
		        "Error assigning memory for decoded output buffer\n");
		/* malloc() failure */
		fl_player_death(-285);
	}
	ansiterm_init();
}

void
fl_clear_line(int x, int y) {
	fl_termcap_cursor_position(1, y);
	*fl_buffer_pointer++ = CL_LINE;
	fl_termcap_cursor_position(x, y);
}

void
fl_clear_screen_from_to(int x, int y) {
	int i;
	fl_termcap_cursor_position(1, 1);
	for (i = 1; i <= y; i++) {
		*fl_buffer_pointer++ = CL_LINE;
		*fl_buffer_pointer++ = '\n';
	}
	fl_termcap_cursor_position(x, y);
}

void
fl_clear_screen_from_to_end_of_display(int x, int y) {
	int i;
	fl_termcap_cursor_position(1, y);
	if (TT_CLEAR_TO_END_OF_DISPLAY == NULL) {
		*fl_buffer_pointer++ = CL_LINE;
		for (i = y; i <= 24; i++) {
			*fl_buffer_pointer++ = CL_LINE;
			if (i != 24) {
				*fl_buffer_pointer++ = '\n';
			}
		}
		fl_termcap_cursor_position(x, y);
	} else {
		*fl_buffer_pointer++ = CL_DOWN;
	}
	fl_termcap_cursor_position(x, y);
}

void
fl_invert_color_space(const char* str) {
	*fl_buffer_pointer++ = ST_START;
	while (*str) {
		*fl_buffer_pointer++ = *str++;
	}
	*fl_buffer_pointer++ = ST_END;
}

static int scrline = 18;	/* line # for wraparound instead of scrolling if no DL */

void
fl_output_buffer_flush(void) {
	Termcap termcap;
	int lpoint;
	char *str;
	static int curx = 0;
	static int cury = 0;
	if ((lpoint = fl_buffer_pointer - lpbuf) > 0) {
		cdesc[FL_BYTES_OUT] += lpoint;
		if (enable_scroll <= -1) {
			flush_buf();
			/*  Catch write errors on save files
			*/
			#if defined WINDOWS
			if (_write(lfd, lpbuf, lpoint) != lpoint)
			#endif
			#if defined NIX || NIX_LOCAL
				if (write(lfd, lpbuf, lpoint) != lpoint)
			#endif
				{
					fprintf(stderr, "Error writing output file\n");
				}
			fl_buffer_pointer = lpbuf;		/* point back to beginning of buffer */
			return;
		}
		for (str = lpbuf; str < fl_buffer_pointer; str++) {
			if (*str >= 32) {
				ttputch(*str);
				curx++;
			} else
				switch (*str) {
					case FL_CLEAR:
						tputs(TT_CLEAR_THE_SCREEN);
						curx = cury = 0;
						break;
					case CL_LINE:
						tputs(TT_CLEAR_TO_END_OF_LINE);
						break;
					case CL_DOWN:
						tputs(TT_CLEAR_TO_END_OF_DISPLAY);
						break;
					case ST_START:
						tputs(TT_BEGIN_STANDARD_OUT_MODE);
						break;
					case ST_END:
						tputs(TT_STOP_STANDARD_OUT_MODE);
						break;
					case FL_CURSOR:
						curx = *++str - 1;
						cury = *++str - 1;
						tputs(termcap.atgoto(TT_CURSOR_MOVE, curx, cury));
						break;
					case '\n':
						if ((cury == 23) && enable_scroll) {
							if (++scrline > 23) {
								scrline = 19;
							}
							tputs(termcap.atgoto(TT_CURSOR_MOVE, 0, scrline + 1));
							tputs(TT_CLEAR_TO_END_OF_LINE);
							tputs(termcap.atgoto(TT_CURSOR_MOVE, 0, scrline));
							tputs(TT_CLEAR_TO_END_OF_LINE);
						} else {
							ttputch('\n');
							cury++;
						}
						curx = 0;
						break;
					case T_INIT:
						tputs(TT_TERMINAL_INITIALIZATION);
						break;
					case T_END:
						tputs(TT_TERMINAL_INITIALIZATION_END);
						break;
					default:
						ttputch(*str);
						curx++;
				}
		}
	}
	fl_buffer_pointer = lpbuf;
	flush_buf();			/* flush real output buffer now */
}

static int io_index = 0;

/*
    ttputch(ch)      Print one character in decoded output buffer.
*/
static void
ttputch(int c) {
	outbuf[io_index++] = static_cast<char>(c);
	if (io_index >= FL_OUTPUT_BUFFER) {
		flush_buf();
	}
}

/*
    Outputs string pointed to by cp.  Modified using public domain termcap
    routines. ~Gibbon.
*/
static void
tputs(const char *cp) {
	if (cp == NULL) {
	}
	while (*cp != '\0') {
		ttputch(*cp++);
	}
}


/*
    Flush buffer with decoded output.

    ~Gibbon
*/
static void
flush_buf(void) {
	if (io_index) {
		if (lfd == 1) {
			ansiterm_out(outbuf, io_index);
		} else {
			write(lfd, outbuf, io_index);
		}
	}
	io_index = 0;
}


/*
    char *tmcapcnv(sd,ss)  Routine to convert VT100 escapes to termcap format

    Processes only the \33[#m sequence (converts . files for termcap use
*/
char *
tmcapcnv(char *sd, char *ss) {
	int tmstate = 0;		/* 0=normal, 1=\33 2=[ 3=# */
	char tmdigit = 0;		/* the # in \33[#m */
	while (*ss) {
		switch (tmstate) {
			case 0:
				if (*ss == '\33') {
					tmstate++;
					break;
				}
				break;
			case 1:
				if (*ss != '[') {
					*sd++ = *ss;
				}
				tmstate++;
				break;
			case 2:
				if (isdigit(*ss)) {
					tmdigit = *ss - '0';
					tmstate++;
					break;
				}
				if (*ss == 'm') {
					*sd++ = ST_END;
					tmstate = 0;
				}
				*sd++ = *ss;
				[[fallthrough]];
			case 3:
				if (*ss == 'm') {
					if (tmdigit) {
						*sd++ = ST_START;
					} else {
						*sd++ = ST_END;
					}
					tmstate = 0;
				}
				[[fallthrough]];
			default:
				*sd++ = *ss;
		};
		ss++;
	}
	*sd = 0;			/* NULL terminator */
	return (sd);
}

void
enter_name(void) {
	int i;
	char c;
	fl_display_message("\n\nEnter character name:\n");
	get_final_char_keyboard = ansiterm_getche;
	i = 0;
	do {
		c = ttgetch();
		if (c == '\n') {
			break;
		}
		if (c == 8) {
			if (i > 0) {
				--i;
				ansiterm_delch();
			}
		} else if (isprint(c)) {
			logname[i] = c;
			++i;
		}
	} while (i < FL_MAX_PLAYER_NAME_LENGTH - 1);
	logname[i] = '\0';
	get_final_char_keyboard = ansiterm_getch;
}
