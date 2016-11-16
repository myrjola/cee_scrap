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

#include "file_writer.hpp"

using std::ofstream;
using std::ios_base;
using std::string;
using std::vector;

FileWriter::FileWriter(void) {
    // Throw exception when operation fails
    _fs.exceptions(ofstream::failbit | ofstream::badbit);
}

FileWriter::~FileWriter(void) {
    try {
        close();
    }
    catch (...) {
        // Do nothing; exceptions should never be thrown from a destructor
    }
}

void FileWriter::open(const std::string& file, bool create_if_missing)
    throw (ios_base::failure)
{
    _fs.open(file.c_str(), ios_base::out | ios_base::binary | ios_base::trunc);
}

void FileWriter::close(void) {
    if (_fs.is_open()) flush();
    _fs.close();
}

void FileWriter::flush(void) throw (ios_base::failure) {
    _fs.flush();
}

void FileWriter::write(const std::string& data) throw (ios_base::failure) {
    _fs << data;
}

void FileWriter::write(const std::vector<char>& data) 
    throw (ios_base::failure)
{
    _fs.write(&data[0], data.size());
}

FileWriter& FileWriter::operator<<(const std::string& data)
    throw (ios_base::failure)
{
    write(data);
    return *this;
}

FileWriter& FileWriter::operator<<(const std::vector<char>& data)
    throw (ios_base::failure)
{
    write(data);
    return *this;
}
