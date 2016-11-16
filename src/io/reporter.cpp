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

#include "reporter.hpp"
#include <iostream>
#include <cstdlib>

using std::cout;
using std::string;

Reporter::Reporter(void) {
    atexit(&cleanup);
}

Reporter::~Reporter(void) {}
    
Reporter* Reporter::getInstance(void) {
    if (!_instance) _instance = new Reporter();
    return _instance;
}

string Reporter::beginInfo(void) const {
    return "";
}

string Reporter::beginError(void) const {
    return "[ERROR] ";
}

Reporter::Sentinel Reporter::endl(void) const {
    return ENDL;
}

void Reporter::flush(void) {
    cout.flush();
}

Reporter& Reporter::operator<<(Sentinel rhs) {
    switch (rhs) {
        case ENDL: {
            cout << std::endl;
            break;
        }

        default: {
            cout << "UNKNOWN SENTINEL" << std::endl;
            break;
        }
    }
    return *this;
}

void Reporter::cleanup(void) {
    delete _instance;
    _instance = 0;
}

Reporter* Reporter::_instance;
