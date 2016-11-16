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

#ifndef CEE_FILE_WRITER__H
#define CEE_FILE_WRITER__H

/**
 * @file
 * @brief Defines the classes for managing file output.
 */

#include <fstream>
#include <ios>
#include <string>
#include <vector>

/**
 * \brief Class for handling file output.
 *
 * The FileWriter class provides methods for writing data to file.
 */
class FileWriter {
  public:
    /**
     * Creates a file writer.
     */
    FileWriter(void);

    /**
     * Destroys this file writer. If a file is opened, it will be closed first.
     */
    ~FileWriter(void);

    /**
     * Opens a specific file. If the file writer already has a file open, that
     * file will be closed first.
     *
     * @param file
     *        File path.
     * @param create_if_missing
     *        Whether to create the file if it does not exist (\c true by
     *        default).
     * @throws std::ios_base::failure
     *         When the operation fails.
     */
    void open(const std::string& file, bool create_if_missing = true)
        throw (std::ios_base::failure);

    /**
     * Closes the open file. If no file is open, this has no effect.
     */
    void close(void);

    /**
     * Writes a string to file.
     *
     * @param data
     *        String of data (as \c char) to write.
     * @throws std::ios_base::failure
     *         When the operation fails.
     */
    void write(const std::string& data) throw (std::ios_base::failure);

    /**
     * Writes a vector of data.
     *
     * @param data
     *        Vector of data to write.
     * @throws std::ios_base::failure
     *         When the operation fails.
     */
    void write(const std::vector<char>& data) throw (std::ios_base::failure);

    /**
     * Flushes all written data to file.
     *
     * @throws std::ios_base::failure
     *         When the operation fails.
     */
    void flush(void) throw (std::ios_base::failure);

    /**
     * \copydoc write(const std::string&).
     *
     * @returns This file writer.
     *
     * Alias for write(const std::string&).
     */
    FileWriter& operator<<(const std::string& data)
        throw (std::ios_base::failure);

    /**
     * \copydoc write(const std::vector<char>&).
     *
     * @returns This file writer.
     *
     * Alias for write(const std::vector<char>&).
     */
    FileWriter& operator<<(const std::vector<char>& data)
        throw (std::ios_base::failure);

  private:
    /**
     * File stream handle.
     */
    std::ofstream _fs;
};

#endif
