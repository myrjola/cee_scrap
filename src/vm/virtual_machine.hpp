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

#ifndef CEE_VM_VIRTUAL_MACHINE__H
#define CEE_VM_VIRTUAL_MACHINE__H

/**
 * @file
 * @brief Defines the classes and functions for the virtual machine.
 */

#include "../decoder/decoder.hpp"
#include "../generator/code_listing.hpp"
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>

/**
 * \brief Implements the virtual machine.
 *
 * The VirtualMachine class implements a stack-based virtual machine which
 * accepts a single code file, formatted according to the instruction set and
 * file format specified in CodeListing.
 *
 * A virtual machine is expected to provide a memory size equal to the size
 * which is specified in the input code. If the code attempts to access a memory
 * location whose index is out of bound, an error is to be reported and the
 * execution halted. The initial content of the memory is undefined.
 */
class VirtualMachine : private Decoder {
  public:
    /**
     * Creates a virtual machine.
     */
    VirtualMachine(void);

    /**
     * Destroys this virtual machine.
     */
    ~VirtualMachine(void);

    /**
     * Executes a program. The method returns once the program has terminated,
     * either due to reaching the end or encountering an error.
     *
     * @param program
     *        Program to execute.
     */
    void execute(const std::vector<char>& program);

  protected:
    /**
     * Resets and prepares the environment to allow execution from a clean
     * state.
     *
     * @returns \c true if the environment was successfully prepared.
     */
    virtual bool prepareEnvironment(void);

    /**
     * Checks that the magic number is correct.
     *
     * @param number
     *        Magic number.
     * @returns \c true if the magic number was correct.
     */
    virtual bool processMagicNumber(int number);

    /**
     * Sets up the main memory.
     *
     * @param value
     *        Number of memory locations needed.
     * @returns Always \c true.
     */
    virtual bool processMemorySize(int value);

    /**
     * \copydoc Decoder::processInstLOAD(void)
     */
    virtual bool processInstLOAD(void);

    /**
     * \copydoc Decoder::processInstSTORE(void)
     */
    virtual bool processInstSTORE(void);

    /**
     * \copydoc Decoder::processInstCONST_1B(char)
     */
    virtual bool processInstCONST_1B(char value);

    /**
     * \copydoc Decoder::processInstCONST_2B(short)
     */
    virtual bool processInstCONST_2B(short value);

    /**
     * \copydoc Decoder::processInstCONST_4B(int)
     */
    virtual bool processInstCONST_4B(int value);

    /**
     * \copydoc Decoder::processInstCONST_0(void)
     */
    virtual bool processInstCONST_0(void);

    /**
     * \copydoc Decoder::processInstCONST_1(void)
     */
    virtual bool processInstCONST_1(void);

    /**
     * \copydoc Decoder::processInstADD(void)
     */
    virtual bool processInstADD(void);

    /**
     * \copydoc Decoder::processInstSUB(void)
     */
    virtual bool processInstSUB(void);

    /**
     * \copydoc Decoder::processInstMUL(void)
     */
    virtual bool processInstMUL(void);

    /**
     * \copydoc Decoder::processInstDIV(void)
     */
    virtual bool processInstDIV(void);

    /**
     * \copydoc Decoder::processInstSWAP(void)
     */
    virtual bool processInstSWAP(void);

    /**
     * \copydoc Decoder::processInstPRINT(void)
     */
    virtual bool processInstPRINT(void);

    /**
     * Prints en error message.
     *
     * @param inst
     *        Byte value of the unknown instruction.
     * @returns Always \c false.
     */
    virtual bool processInstUnknown(char inst);


  private:
    /* TASK: Add whatever private members and methods you find necessary */
};

#endif
