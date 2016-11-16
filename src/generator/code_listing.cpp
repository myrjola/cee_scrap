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

#include "code_listing.hpp"
#include <climits>
#include <sstream>

using std::string;
using std::stringstream;
using std::vector;

CodeListing::CodeListing() : _num_memory_locations(0) {}

CodeListing::~CodeListing(void) {}

void CodeListing::setNumMemoryLocations(int num) {
    _num_memory_locations = num;
}

void CodeListing::generateInitCode(void) {
    appendConstValue(static_cast<int>(0x1337D00D));
    appendConstValue(switchEndianInt(_num_memory_locations));
}

void CodeListing::appendInstruction(Instruction inst) {
    _code.push_back(static_cast<char>(inst));
}

void CodeListing::appendConstValue(char value) {
    _code.push_back(value);
}

void CodeListing::appendConstValue(short value) {
    _code.push_back(static_cast<char>(value >> 8));
    _code.push_back(static_cast<char>(value));
}

void CodeListing::appendConstValue(int value) {
    _code.push_back(static_cast<char>(value >> 24));
    _code.push_back(static_cast<char>(value >> 16));
    _code.push_back(static_cast<char>(value >>  8));
    _code.push_back(static_cast<char>(value));
}

CodeListing& CodeListing::operator<<(Instruction rhs) {
    appendInstruction(rhs);
    return *this;
}

CodeListing& CodeListing::operator<<(char rhs) {
    appendConstValue(rhs);
    return *this;
}

CodeListing& CodeListing::operator<<(short rhs) {
    appendConstValue(rhs);
    return *this;
}

CodeListing& CodeListing::operator<<(int rhs) {
    appendConstValue(rhs);
    return *this;
}

const vector<char>& CodeListing::getCode(void) const {
    return _code;
}

bool CodeListing::willFitInChar(int value) {
    return value >= SCHAR_MIN && value <= SCHAR_MAX;
}

bool CodeListing::willFitInShort(int value) {
    return value >= SHRT_MIN && value <= SHRT_MAX;
}

short CodeListing::switchEndianShort(short value) {
    /* TASK: Implement this method */
}

int CodeListing::switchEndianInt(int value) {
    /* TASK: Implement this method */
}

int CodeListing::toInt(const string& str) {
    stringstream ss(str);
    int value;
    ss >> value;
    return value;
}
