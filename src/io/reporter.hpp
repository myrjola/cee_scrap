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

#ifndef CEE_REPORTER__H
#define CEE_REPORTER__H

/**
 * @file
 * @brief Defines the common interface for information and error reporting.
 */

#include <iostream>
#include <string>

/**
 * \brief Common interface for information and error reporting.
 *
 * The Reporter class provides a common interface for reporting information to
 * the user. The messages will be written to standard out, and the class applies
 * the <a
 * href="http://en.wikipedia.org/wiki/Singleton_pattern"><em>Singleton</em></a>
 * design pattern in order to ensure a single instance of the reporter.
 */
class Reporter {
  public:
    /**
     * Sentinels for production of special reporter output.
     */
    enum Sentinel {
        /**
         * New line and flush the reporter.
         */
        ENDL
    };

  public:
    /**
     * Gets a Reporter instance.
     *
     * @returns Reporter instance.
     */
    static Reporter* getInstance(void);

    /**
     * Begins an info print string.
     *
     * @returns Beginning of info print string.
     */
    std::string beginInfo(void) const;

    /**
     * Begins an error print string.
     *
     * @returns Beginning of error print string.
     */
    std::string beginError(void) const;

    /**
     * Returns a sentinel for printing a new line and flushing the reporter.
     *
     * @returns \c ENDL sentinel.
     */
    Sentinel endl(void) const;

    /**
     * Flushes the reporter.
     */
    void flush(void);

    /**
     * Processes a sentinel.
     *
     * @returns This reporter.
     */
    Reporter& operator<<(Sentinel rhs);

    /**
     * Prints \c rhs to standard output.
     *
     * @param rhs
     *        Object or data to print.
     * @returns This reporter.
     */
    template <class T>
    Reporter& operator<<(const T& rhs) {
        std::cout << rhs;
        return *this;
    }
  private:
    /**
     * Create a reporter. This is hidden in order to prevent additional
     * instances from being created.
     */
    Reporter(void);

    /**
     * Destroys this reporter.
     */
    ~Reporter(void);

    /**
     * Copies a reporter. This is hidden in order to prevent additional
     * instances from being created.
     */
    Reporter(const Reporter&);

    /**
     * Assigns a reporter to another. This is hidden in order to prevent
     * additional instances from being created.
     *
     * @returns This instance.
     */
    Reporter& operator=(const Reporter&);

    /**
     * Destroys the static reporter instance.
     */
    static void cleanup(void);

  private:
    /**
     * Reporter instance.
     */
    static Reporter* _instance;
};

#endif
