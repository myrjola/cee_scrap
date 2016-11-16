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

#ifndef CEE_FILE_READER__H
#define CEE_FILE_READER__H

/**
 * @file
 * @brief Defines the classes for managing file input.
 */

#include <fstream>
#include <ios>
#include <string>
#include <vector>

/**
 * \brief Class for handling file input.
 *
 * The FileWriter class provides methods for reading data to file.
 */
class FileReader {
  public:
    /**
     * Creates a file reader.
     */
    FileReader(void);

    /**
     * Destroys this file reader. If a file is opened, it will be closed first.
     */
    ~FileReader(void);

    /**
     * Opens a specific file. If the file reader already has a file open, that
     * file will be closed first.
     *
     * @param file
     *        File path.
     * @throws std::ios_base::failure
     *         When the operation fails.
     */
    void open(const std::string& file) throw (std::ios_base::failure);

    /**
     * Closes the open file. If no file is open, this has no effect.
     */
    void close(void);

    /**
     * Reads data into a vector. The data will be appended to the vector.
     *
     * @param dest
     *        Destination vector to where the read data will be stored.
     * @param num_bytes
     *        Number of bytes to read. If there are less bytes to read than the
     *        desired number, only the remaining part will be read. Default
     *        setting is to read the entire file.
     * @throws std::ios_base::failure
     *         When the operation fails.
     */
    void read(std::vector<char>* dest, int num_bytes = -1)
        throw (std::ios_base::failure);

    /**
     * Reads entire remaining content of file into a vector.
     *
     * @param dest
     *        Destination vector to where the read data will be stored.
     * @returns This file reader.
     * @throws std::ios_base::failure
     *         When the operation fails.
     *
     * Alias for read(std::vector<char>*).
     */
    FileReader& operator>>(std::vector<char>& dest)
        throw (std::ios_base::failure);

  private:
    /**
     * Copies the content of the buffer to the destination vector.
     *
     * @param dest
     *        Destination vector.
     */
    void copyBufToDest(std::vector<char>* dest);

  private:
    /**
     * Size of the buffer.
     */
    static const int BUF_SIZE;

    /**
     * File stream handle.
     */
    std::ifstream _fs;

    /**
     * Input buffer.
     */
    char _buf[1024];  // C++ doesn't allow usage of BUF_SIZE to declare this

    /**
     * Numbers of bytes read by the last operation.
     */
    int _num_bytes_read;
};

#endif
