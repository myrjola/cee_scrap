#
#  Copyright:
#     Gabriel Hjort Blindell, 2012
#
#  Permission is hereby granted, free of charge, to any person obtaining
#  a copy of this software and associated documentation files (the
#  "Software"), to deal in the Software without restriction, including
#  without limitation the rights to use, copy, modify, merge, publish,
#  distribute, sublicense, and/or sell copies of the Software, and to
#  permit persons to whom the Software is furnished to do so, subject to
#  the following conditions:
#
#  The above copyright notice and this permission notice shall be
#  included in all copies or substantial portions of the Software.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
#  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
#  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
#  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
#  LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
#  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
#  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#

# Settings
EXECUTABLE = scanner
SCANNER_INPUT_FILE  = ../../grammar/scanner.l
SCANNER_OUTPUT_FILE = ../../grammar/lex.yy.c
SCANNER_HEADER_FILE = ../../grammar/lex.yy.h
PARSER_HEADER_FILE = ../../grammar/parser.tab.h
C_SOURCES = $(SCANNER_OUTPUT_FILE)
CPP_SOURCES = main.cpp ../../io/reporter.cpp

# Linux
GCCCPP = g++
GCCCPPFLAGS = -Wall
GCCLINKFLAGS = -Wall
LINUXOBJECTS = $(C_SOURCES:.c=.o) $(CPP_SOURCES:.cpp=.o)

# Targets
all: linux

linux: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(LINUXOBJECTS)
	$(GCCCPP) $(GCCLINKFLAGS) $(LINUXOBJECTS) -o $@
	@printf "BUILD OK\n"

.cpp.o:
	$(GCCCPP) $(GCCCPPFLAGS) -c $< -o $@

.c.o:
	$(GCCCPP) $(GCCCPPFLAGS) -c $< -DSCANNER_BUILD -o $@

$(SCANNER_OUTPUT_FILE): $(SCANNER_INPUT_FILE) $(PARSER_HEADER_FILE)
	flex --header-file=$(SCANNER_HEADER_FILE) -o $(SCANNER_OUTPUT_FILE) $<

$(PARSER_HEADER_FILE):
	cp $(notdir $(PARSER_HEADER_FILE)) $(PARSER_HEADER_FILE)

clean:
	-rm $(PARSER_HEADER_FILE)
	-rm $(SCANNER_OUTPUT_FILE)
	-rm $(SCANNER_HEADER_FILE)
	-rm $(LINUXOBJECTS)

distclean: clean
	-rm $(EXECUTABLE)

.PHONE: clean
