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

#ifndef CEE_GENERATOR_CODE_LISTING__H
#define CEE_GENERATOR_CODE_LISTING__H

/**
 * @file
 * @brief Defines the classes and functions for managing code generation.
 */

#include <string>
#include <vector>

/**
 * \brief Code generation interface.
 *
 * The class serves two purposes:
 *     -# provide an interface for producing instructions, and
 *     -# act as a container for the generated code.
 *
 * Instructions are appended to the code listing, and once done, the entire
 * listing can be retrieved as a string which can then be written to file.
 *
 * The code listing targets a stack-based virtual machine. During execution,
 * the machine maintains a main memory and a stack. The instructions operate
 * on the values on the stack, which may stored intermediately in the main
 * memory. A value on the stack is called an \e operand.
 *
 * The instructions are described according to the following schema:
 *     - <b>Use:</b> &lt;brief description&gt;
 *     - <b>Description:</b> &lt;detailed description&gt;
 *     - <b>Number of operands:</b> &lt;number&gt;
 *     - <b>Stack before:</b> [\e op1, \e op2, \e op3, ...]
 *     - <b>Stack after:</b> [\e v1, \e v2, \e v3, ...]
 *
 * The left-most part of the stack denotes the \e top of the stack, and thus
 * should be read from left to right as that denotes the popping order. Only the
 * operands of interest will be shown in the stack - other values that may or
 * may not be already in the stack are omitted from the description.
 *
 * Most instructions are self-explanatory, but #CONST_1B, #CONST_2B, #CONST_4B
 * may need some clarification. Since the machine is entirely stack-based, all
 * values must in some way be pushed to the stack. If the value resides in a
 * memory then this is no problem, but if the value does not then some other
 * mechanism must be applied in order to push the value. The approach used here
 * is to embed the constants into the code. Special instructions are then used
 * to treat the following instructions as values, and push them onto the stack.
 * As a side-effect, the program counter is forwarded in order to prevent the
 * machine from interpreting the values as instructions. Values are stored in
 * <a href="http://en.wikipedia.org/wiki/Endianness#Big-endian">
 * <em>big-endian</em></a> (remember that Intel x86 and x64 use
 * <a href="http://en.wikipedia.org/wiki/Endianness#Little-endian">
 * <em>little-endian</em></a>).
 *
 * The code listing will adhere to the following structure:
 *     - Magic number \c 0x1337D00D, followed by
 *     - Number of memory locations used (as \c int, big-endian), followed by
 *     - Code (as a series of \c char values)
 *
 * The machine is expected to halt and terminate upon reaching the final
 * instruction.
 */
class CodeListing {
  public:
    /**
     * Defines the instruction set. The \c enum value is also the value of the
     * instruction when stored in the program code space.
     */
    enum Instruction {
        /**
         * - <b>Use:</b> Pushes the value at a memory location onto the stack.
         * - <b>Description:</b> The instruction pops 1 value from the stack,
         *                       which is the memory index. The value in the
         *                       memory is then pushed onto the stack.
         * - <b>Number of operands:</b> 1
         * - <b>Stack before:</b> \e index
         * - <b>Stack after:</b> \e value
         */
        LOAD = 1,

        /**
         * - <b>Use:</b> Stores the top value from the stack into a memory
         *               location.
         * - <b>Description:</b> The instruction pops 2 values from the stack,
         *                       a memory index and a value. The value is then
         *                       stored in the memory location.
         * - <b>Number of operands:</b> 2
         * - <b>Stack before:</b> \e index \e value
         * - <b>Stack after:</b> 
         */
        STORE = 2,

        /**
         * - <b>Use:</b> Pushes a \c char constant onto the stack.
         * - <b>Description:</b> The instruction pushes the 1-byte value that
         *                       follows the instruction onto the stack. The
         *                       program counter is then incremented such as to
         *                       bypass the value.
         * - <b>Number of operands:</b> 0
         * - <b>Stack before:</b> 
         * - <b>Stack after:</b> \e value
         */
        CONST_1B = 3,

        /**
         * - <b>Use:</b> Pushes a \c short constant onto the stack.
         * - <b>Description:</b> The instruction pushes the 2-byte value that
         *                       follows the instruction onto the stack. The
         *                       program counter is then incremented such as to
         *                       bypass the value.
         * - <b>Number of operands:</b> 0
         * - <b>Stack before:</b> 
         * - <b>Stack after:</b> \e value
         */
        CONST_2B = 4,

        /**
         * - <b>Use:</b> Pushes an \c int constant onto the stack.
         * - <b>Description:</b> The instruction pushes the 4-byte value that
         *                       follows the instruction onto the stack. The
         *                       program counter is then incremented such as to
         *                       bypass the value.
         * - <b>Number of operands:</b> 0
         * - <b>Stack before:</b> 
         * - <b>Stack after:</b> \e value
         */
        CONST_4B = 5,

        /**
         * - <b>Use:</b> Pushes the value 0 onto the stack.
         * - <b>Description:</b> The instruction pushes the constant value 0 
         *                       onto the stack.
         * - <b>Number of operands:</b> 0
         * - <b>Stack before:</b> 
         * - <b>Stack after:</b> 0
         */
        CONST_0 = 12,

        /**
         * - <b>Use:</b> Pushes the value 1 onto the stack.
         * - <b>Description:</b> The instruction pushes the constant value 1
         *                       onto the stack.
         * - <b>Number of operands:</b> 0
         * - <b>Stack before:</b> 
         * - <b>Stack after:</b> 1
         */
        CONST_1 = 13,

        /**
         * - <b>Use:</b> Produces the sum of two values from the stack.
         * - <b>Description:</b> The instruction pops the 2 top-most values from
         *                       the stack, calculates the sum (\e v1 + \e v2), 
         *                       and pushes the result back onto the stack.
         * - <b>Number of operands:</b> 2
         * - <b>Stack before:</b> \e v2 \e v1
         * - <b>Stack after:</b> \e sum
         */
        ADD = 6,

        /**
         * - <b>Use:</b> Produces the difference of two values from the stack.
         * - <b>Description:</b> The instruction pops the 2 top-most values from
         *                       the stack, calculates the difference
         *                       (\e v1 - \e v2), and pushes the result back
         *                       onto the stack.
         * - <b>Number of operands:</b> 2
         * - <b>Stack before:</b> \e v2 \e v1
         * - <b>Stack after:</b> \e diff
         */
        SUB = 7,

        /**
         * - <b>Use:</b> Produces the product of two values from the stack.
         * - <b>Description:</b> The instruction pops the 2 top-most values from
         *                       the stack, calculates the product
         *                       (\e v1 * \e v2), 
         *                       and pushes the result back onto the stack.
         * - <b>Number of operands:</b> 2
         * - <b>Stack before:</b> \e v2 \e v1
         * - <b>Stack after:</b> \e prod
         */
        MUL = 8,

        /**
         * - <b>Use:</b> Produces the quotient of two values from the stack.
         * - <b>Description:</b> The instruction pops the 2 top-most values from
         *                       the stack, calculates the quotient
         *                       (\e v1 / \e v2), and pushes the result back
         *                       onto the stack.
         * - <b>Number of operands:</b> 2
         * - <b>Stack before:</b> \e v2 \e v1
         * - <b>Stack after:</b> \e quot
         */
        DIV = 9,

        /**
         * - <b>Use:</b> Swaps the two top-most values on the stack.
         * - <b>Description:</b> The instruction pops the 2 top-most values from
         *                       the stack, and pushes them back in the reversed
         *                       order.
         * - <b>Number of operands:</b> 2
         * - <b>Stack before:</b> \e v1 \e v2
         * - <b>Stack after:</b> \e v2 \e v1
         */
        SWAP = 10,

        /**
         * - <b>Use:</b> Prints the top-most value on the stack.
         * - <b>Description:</b> The instruction pops the top-most value from
         *                       the stack, and prints the value onto the
         *                       standard output.
         * - <b>Number of operands:</b> 1
         * - <b>Stack before:</b> \e value
         * - <b>Stack after:</b>
         */
        PRINT = 11
    };

  public:
    /**
     * Creates a code listing.
     */
    CodeListing();

    /**
     * Destroys this code listing.
     */
    ~CodeListing(void);

    /**
     * Sets the number of memory locations that will be used within this code
     * listing. This number must not change after generateInitCode() has been
     * invoked!
     * 
     * By default, this value is 0.
     *
     * @param num
     *        Number of memory locations to use.
     */
    void setNumMemoryLocations(int num);

    /**
     * Writes the magic number and number of memory locations to use to the code
     * listing.
     */
    void generateInitCode(void);

    /**
     * Appends an instruction to this code listing. Using
     * operator<<(Instruction) has the same effect.
     *
     * @param inst
     *        Instruction.
     */
    void appendInstruction(Instruction inst);

    /**
     * Appends a 1-byte value to this code listing.
     *
     * @param value
     *        1-byte const value.
     */
    void appendConstValue(char value);

    /**
     * Appends a 2-byte value to this code listing.
     *
     * @param value
     *        2-byte const value.
     */
    void appendConstValue(short value);

    /**
     * Appends a 4-byte value to this code listing.
     *
     * @param value
     *        4-byte const value.
     */
    void appendConstValue(int value);

    /**
     * Appends an instruction to this code listing.
     *
     * @param rhs
     *        Instruction.
     * @returns This code listing.
     *
     * Alias for appendInstruction(Instruction).
     */
    CodeListing& operator<<(Instruction rhs);

    /**
     * Appends a 1-byte value to this code listing.
     *
     * @param rhs
     *        1-byte const value.
     * @returns This code listing.
     *
     * Alias for appendConstValue(char).
     */
    CodeListing& operator<<(char rhs);

    /**
     * Appends a 2-byte value to this code listing.
     *
     * @param rhs
     *        2-byte const value.
     * @returns This code listing.
     *
     * Alias for appendConstValue(short).
     */
    CodeListing& operator<<(short rhs);

    /**
     * Appends a 4-byte value to this code listing.
     *
     * @param rhs
     *        4-byte const value.
     * @returns This code listing.
     *
     * Alias for appendConstValue(int).
     */
    CodeListing& operator<<(int rhs);

    /**
     * Returns the code as a vector of \c char values. This can then be
     * directly written to file. The vector is returned by reference in order
     * to avoid redundant copying.
     *
     * @returns Code vector.
     */
    const std::vector<char>& getCode(void) const;

    /**
     * Checks whether the value can be stored using just a \c char.
     *
     * @param value
     *        Value.
     * @returns \c true if the value is small enough.
     */
    static bool willFitInChar(int value);

    /**
     * Checks whether the value can be stored using just a \c short.
     *
     * @param value
     *        Value.
     * @returns \c true if the value is small enough.
     */
    static bool willFitInShort(int value);

    /**
     * Switches a \c short from one endian (little or big) to another.
     *
     * @param value
     *        Value to switch endianess.
     * @returns New value.
     */
    static short switchEndianShort(short value);

    /**
     * Switches a \c int from one endian (little or big) to another.
     *
     * @param value
     *        Value to switch endianess.
     * @returns New value.
     */
    static int switchEndianInt(int value);

    /**
     * Converts a \c string into an \c int.
     *
     * @param str
     *        String to convert.
     * @returns Value as \c int.
     */
    static int toInt(const std::string& str);

  private:
    /**
     * Contains the generated code.
     */
    std::vector<char> _code;

    /**
     * Determines the number of memory locations needed.
     */
    int _num_memory_locations;
};

#endif
