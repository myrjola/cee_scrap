/*
 *  Copyright:
 *     Gabriel Hjort Blindell, 2012
 *
 *  Permission is hereby granted, free of charge, to any person obtaining
 *  a copy of this software and associated documentation files (the
 *  "Software"), to deal in the Software without restriction, including
 *  without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to
 *  permit persons to whom the Software is furnished to do so, subject to
 *  the following conditions:
 *
 *  The above copyright notice and this permission notice shall be
 *  included in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 *  LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 *  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 *  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

/*
 * USE: For testing the decoder. It accepts a compiled program file as input and
 * prints its content to the standard output.
 */

#include "../../decoder/decoder.hpp"
#include "../../io/file_reader.hpp"
#include "../../io/reporter.hpp"
#include <ios>
#include <sstream>
#include <string>
#include <vector>

using std::hex;
using std::ios_base;
using std::string;
using std::stringstream;
using std::vector;

/**
 * Implements a decoder which prints the content of a program.
 */
class ProgramPrinter : private Decoder {
  public:
    ProgramPrinter(void) : _out(*Reporter::getInstance()) {}

    void print(const std::vector<char>& program) {
        invoke(program);
    }

  protected:
    virtual bool prepareEnvironment(void) {
        _out << _out.beginInfo() << "PROGRAM INFO:" << _out.endl()
             << "Total code size: " << getProgramSize() << " bytes"
             << _out.endl();
        return true;
    }

    virtual bool processMagicNumber(int value) {
        stringstream ss;
        ss << "Magic value: 0x" << hex << value;
        _out << _out.beginInfo() << ss.str() << _out.endl();
        return true;
    }

    virtual bool processMemorySize(int value) {
        _out << _out.beginInfo() << "Memory size (number of 4-byte values): "
             << value << _out.endl();
        return true;
    }

    virtual bool beforeCodeExecution(void) {
        _out << _out.endl()
             << _out.beginInfo() << "CODE:" << _out.endl();
        return true;
    }

    virtual bool processInstLOAD(void) {
        _out << _out.beginInfo() << padLine(getPC(), getPCAtEndOfProgram())
             << ": LOAD" << _out.endl();
        return true;
    }

    virtual bool processInstSTORE(void) {
        _out << _out.beginInfo() << padLine(getPC(), getPCAtEndOfProgram())
             << ": STORE" << _out.endl();
        return true;
    }

    virtual bool processInstCONST_1B(char value) {
        _out << _out.beginInfo() << padLine(getPC(), getPCAtEndOfProgram())
             << ": CONST_1B (" << static_cast<int>(value) << ")" << _out.endl();
        return true;
    }

    virtual bool processInstCONST_2B(short value) {
        _out << _out.beginInfo() << padLine(getPC(), getPCAtEndOfProgram())
             << ": CONST_2B (" << static_cast<int>(value) << ")" << _out.endl();
        return true;
    }

    virtual bool processInstCONST_4B(int value) {
        _out << _out.beginInfo() << padLine(getPC(), getPCAtEndOfProgram())
             << ": CONST_4B (" << value << ")" << _out.endl();
        return true;
    }

    virtual bool processInstCONST_0(void) {
        _out << _out.beginInfo() << padLine(getPC(), getPCAtEndOfProgram())
             << ": CONST_0" << _out.endl();
        return true;
    }

    virtual bool processInstCONST_1(void) {
        _out << _out.beginInfo() << padLine(getPC(), getPCAtEndOfProgram())
             << ": CONST_1" << _out.endl();
        return true;
    }

    virtual bool processInstADD(void) {
        _out << _out.beginInfo() << padLine(getPC(), getPCAtEndOfProgram())
             << ": ADD" << _out.endl();
        return true;
    }

    virtual bool processInstSUB(void) {
        _out << _out.beginInfo() << padLine(getPC(), getPCAtEndOfProgram())
             << ": SUB" << _out.endl();
        return true;
    }

    virtual bool processInstMUL(void) {
        _out << _out.beginInfo() << padLine(getPC(), getPCAtEndOfProgram())
             << ": MUL" << _out.endl();
        return true;
    }

    virtual bool processInstDIV(void) {
        _out << _out.beginInfo() << padLine(getPC(), getPCAtEndOfProgram())
             << ": DIV" << _out.endl();
        return true;
    }

    virtual bool processInstSWAP(void) {
        _out << _out.beginInfo() << padLine(getPC(), getPCAtEndOfProgram())
             << ": SWAP" << _out.endl();
        return true;
    }

    virtual bool processInstPRINT(void) {
        _out << _out.beginInfo() << padLine(getPC(), getPCAtEndOfProgram())
             << ": PRINT" << _out.endl();
        return true;
    }

    virtual bool processInstUnknown(char inst) {
        stringstream ss;
        ss << "0x" << std::hex << (short) (0x00FF & inst);
        _out << _out.beginInfo() << padLine(getPC(), getPCAtEndOfProgram())
             << ": Unknown instruction (" << ss.str() << ")"
             << _out.endl();
        return true;
    }

  private:
    /**
     * Pads a line number such that all printed numbers are right-aligned.
     *
     * @param line
     *        Line to pad.
     * @param last_line
     *        Last line to print.
     * @returns Padded line.
     */
    static std::string padLine(unsigned int line, unsigned int last_line) {
        // There doesn't seem to be a way to clear a stringstream, so we need to
        // use two separate instances for each int-to-string conversion
        string line_str, last_line_str;
        stringstream ss1, ss2;
        ss1 << line;
        line_str = ss1.str();
        ss2 << last_line;
        last_line_str = ss2.str();

        string str(last_line_str.length() - line_str.length(), ' ');
        str += line_str;
        return str;
    }

  private:
    Reporter& _out;
};

int main(int argc, char** argv) {
    Reporter& out = *Reporter::getInstance();

    // Check command-line
    if (argc > 2) {
        out << out.beginError() << "Too many arguments. Use \"-h\" for help."
            << out.endl();
        return 1;
    }
    else if (argc < 2) {
        out << out.beginError() << "Too few arguments. Use \"-h\" for help."
            << out.endl();
        return 1;
    }

    // Parse command-line
    string program_file;
    if (argv[1][0] == '-') {
        string argument = argv[1];
        if (argument == "-h" || argument == "--help") {
            out << "Usage: " << argv[0] << " [-h] [--help] INPUT_FILE"
                << out.endl();
            return 0;
        }
        else {
            out << out.beginError() << "Invalid option. Use \"-h\" for help."
                << out.endl();
        }
    }
    else {
        program_file = argv[1];
    }

    // Read program file
    FileReader reader;
    try {
        reader.open(program_file);
    }
    catch (ios_base::failure) {
        out << out.beginError() << "Failed to open input file" << out.endl();
    }
    vector<char> program;
    try {
        reader >> program;
    }
    catch (ios_base::failure) {
        out << out.beginError() << "Failed to read input file" << out.endl();
    }

    // Run decoder
    ProgramPrinter decoder;
    decoder.print(program);

    return 0;
}
