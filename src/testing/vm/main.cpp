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
 * USE: For testing the virtual machine. It reads a given compiled program file,
 * and executes it.
 */

#include "../../io/file_reader.hpp"
#include "../../io/reporter.hpp"
#include "../../vm/virtual_machine.hpp"
#include <ios>
#include <string>
#include <vector>

using std::ios_base;
using std::string;
using std::vector;

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
            return 1;
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
        return 1;
    }
    vector<char> program;
    try {
        reader >> program;
    }
    catch (ios_base::failure) {
        out << out.beginError() << "Failed to read input file" << out.endl();
        return 1;
    }

    // Run virtual machine
    VirtualMachine vm;
    vm.execute(program);

    return 0;
}
