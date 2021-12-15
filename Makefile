CC = g++
CC_FLAGS = -std=c++11 -Winline -fno-elide-constructors -pipe -Wall -Werror -pedantic-errors -Wpointer-arith -Woverloaded-virtual -Wshadow -Wmissing-declarations -fomit-frame-pointer -DNIX_LOCAL

EXEC = freelarn
SOURCES = $(wildcard src/*.cpp) $(wildcard src/**/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
LIBS = -lncurses

$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC) $(LIBS)
	mv $(EXEC) bin/

%.o: %.cpp
	$(CC) -c $(CC_FLAGS) $< -o $@

clean:
	rm -f $(EXEC) $(OBJECTS)
