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

#include "file_reader.hpp"
#include <algorithm>

using std::ifstream;
using std::ios;
using std::ios_base;
using std::min;
using std::streampos;
using std::string;
using std::vector;

FileReader::FileReader(void) : _num_bytes_read(0) {
    // Throw exception when operation fails
    _fs.exceptions(ifstream::failbit | ifstream::badbit);
}

FileReader::~FileReader(void) {
    try {
        close();
    }
    catch (...) {
        // Do nothing; exceptions should never be thrown from a destructor
    }
}

void FileReader::open(const std::string& file) throw (ios_base::failure) {
    _fs.open(file.c_str(), ios_base::in | ios_base::binary);
}

void FileReader::close(void) {
    _fs.close();
    _num_bytes_read = 0;
}

void FileReader::read(std::vector<char>* dest, int num_bytes)
    throw (ios_base::failure)
{
    // Find out how many bytes to read
    int num_bytes_remaining;
    streampos old_pos = _fs.tellg();
    _fs.seekg(0, ios::end);
    int num_bytes_left_in_file = _fs.tellg() - old_pos;
    _fs.seekg(old_pos);
    if (num_bytes < 0) {
        num_bytes_remaining = num_bytes_left_in_file;
    }
    else {
        num_bytes_remaining = min(num_bytes, num_bytes_left_in_file);
    }

    // Read data
    while (num_bytes_remaining > 0) {
        int num_bytes_to_read = min(BUF_SIZE, num_bytes_remaining);
        _fs.read(_buf, num_bytes_to_read);
        _num_bytes_read = _fs.gcount();
        copyBufToDest(dest);
        num_bytes_remaining -= _num_bytes_read;
        _num_bytes_read = 0;
    }
}

FileReader& FileReader::operator>>(std::vector<char>& dest)
    throw (ios_base::failure)
{
    read(&dest);
    return *this;
}

void FileReader::copyBufToDest(std::vector<char>* dest) {
    for (int offset = 0; offset < _num_bytes_read; offset++) {
        dest->push_back(_buf[offset]);
    }
}

const int FileReader::BUF_SIZE = 1024;
