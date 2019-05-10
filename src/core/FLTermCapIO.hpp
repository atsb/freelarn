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

void fl_setup_terminal(void);
void fl_clear_terminal(void);
char ttgetch(void);
void lprintf(const char *, ...);
void fl_display_message(const char *, ...);
void lprint(int);
void fl_output_single_byte_to_output_buffer(char);
void fl_write_current_buffer_to_output_buffer(char *, int);
char fl_read_char_from_input_buffer(void);
int larint(void);
void lrfill(char *, int);
char *lgetw(void);
char *lgetl(void);
int fl_create_new_file(char *);
int fl_open_file(char *);
int fl_append_to_file(char *);
void fl_close_the_file(void);
void fl_close_file_flush(void);
void fl_termcap_cursor_position(int, int);
void fl_initialize_termcap_terminal_handling(void);
void fl_clear_line(int, int);
void fl_clear_screen_from_to(int, int);
void fl_clear_screen_from_to_end_of_display(int, int);
void fl_invert_color_space(const char*);
void fl_output_buffer_flush(void);
char *tmcapcnv(char *, char *);
void enter_name(void);

/* size of the lgetw() and lgetl() buffer */
inline const int LINBUFSIZE = 128;