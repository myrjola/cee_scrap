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
EXECUTABLE = vm
CPP_SOURCES = main.cpp ../../io/reporter.cpp ../../io/file_reader.cpp \
              ../../decoder/decoder.cpp ../../generator/code_listing.cpp \
              ../../vm/virtual_machine.cpp

# Linux
GCCCPP = g++
GCCCPPFLAGS = -Wall
GCCLINKFLAGS = -Wall
LINUXOBJECTS = $(CPP_SOURCES:.cpp=.o)

# Targets
all: linux

linux: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(LINUXOBJECTS)
	$(GCCCPP) $(GCCLINKFLAGS) $(LINUXOBJECTS) -o $@
	@printf "BUILD OK\n"

.cpp.o:
	$(GCCCPP) $(GCCCPPFLAGS) -c $< -o $@

clean:
	-rm $(LINUXOBJECTS)

distclean: clean
	-rm $(EXECUTABLE)

.PHONE: clean
