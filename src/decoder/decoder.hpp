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

#ifndef CEE_DECODER_DECODER__H
#define CEE_DECODER_DECODER__H

/**
 * @file
 * @brief Defines the classes and functions for a decoder.
 */

#include "../generator/code_listing.hpp"
#include <vector>

/**
 * \brief Implements a decoder.
 *
 * The Decoder class provides methods for accepting a compiled program and
 * decoding it. The actual execution of the program is handled by hooks which
 * interprets a given instruction. By deriving this class, the same base code
 * can be used in different settings (e.g. printing the program content, or
 * executing it). All hooks return a Boolean value, indicating whether the
 * processing was successful. If \c false is returned, all further processing is
 * halted and the decoder returns from invoke(const std::vector<char>&).
 */
class Decoder {
  public:
    /**
     * Creates a decoder.
     */
    Decoder(void);

    /**
     * Destroys this decoder.
     */
    virtual ~Decoder(void);

    /**
     * Invokes the decoder on a given program.
     *
     * @param program
     *        Program.
     */
    void invoke(const std::vector<char>& program);

  protected:
    /**
     * Prepares the environment. This is the first hook invoked.
     *
     * @returns \c true if the environment was successfully prepared.
     */
    virtual bool prepareEnvironment(void) = 0;

    /**
     * Processes the magic number found in the program.
     *
     * @param number
     *        Found magic number.
     * @returns \c true if the magic number was accepted.
     */
    virtual bool processMagicNumber(int number) = 0;

    /**
     * Processes the memory size value found in the program. The value denotes
     * the number of 4-byte memory entities to create.
     *
     * <b>NOTE:</b> This value must have been converted from big-endian into
     * little-endian before invoking this method.
     *
     * @param value
     *        Found memory size value.
     * @returns \c true if the value was accepted.
     */
    virtual bool processMemorySize(int value) = 0;

    /**
     * This hook is invoked after the magic number and memory size have been
     * processed, but before the actual code is executed. By default this does
     * nothing.
     *
     * @returns \c true if the call was successful.
     */
    virtual bool beforeCodeExecution(void);

    /**
     * This hook is invoked after the code has finished executing. This is not
     * invoked the execution is terminated by some error. By default this does
     * nothing.
     *
     * @returns \c true if the call was successful.
     */
    virtual bool afterCodeExecution(void);

    /**
     * Processes a CodeListing::LOAD instruction.
     *
     * @returns \c true if the instruction was successfully processed.
     */
    virtual bool processInstLOAD(void) = 0;

    /**
     * Processes a CodeListing::STORE instruction.
     *
     * @returns \c true if the instruction was successfully processed.
     */
    virtual bool processInstSTORE(void) = 0;

    /**
     * Processes a CodeListing::CONST_1B instruction.
     *
     * @param value
     *        Constant value (in little-endian).
     * @returns \c true if the instruction was successfully processed.
     */
    virtual bool processInstCONST_1B(char value) = 0;

    /**
     * Processes a CodeListing::CONST_2B instruction.
     *
     * @param value
     *        Constant value (in little-endian).
     * @returns \c true if the instruction was successfully processed.
     */
    virtual bool processInstCONST_2B(short value) = 0;

    /**
     * Processes a CodeListing::CONST_4B instruction.
     *
     * @param value
     *        Constant value (in little-endian).
     * @returns \c true if the instruction was successfully processed.
     */
    virtual bool processInstCONST_4B(int value) = 0;

    /**
     * Processes a CodeListing::CONST_0 instruction.
     *
     * @returns \c true if the instruction was successfully processed.
     */
    virtual bool processInstCONST_0(void) = 0;

    /**
     * Processes a CodeListing::CONST_1 instruction.
     *
     * @returns \c true if the instruction was successfully processed.
     */
    virtual bool processInstCONST_1(void) = 0;

    /**
     * Processes a CodeListing::ADD instruction.
     *
     * @returns \c true if the instruction was successfully processed.
     */
    virtual bool processInstADD(void) = 0;

    /**
     * Processes a CodeListing::SUB instruction.
     *
     * @returns \c true if the instruction was successfully processed.
     */
    virtual bool processInstSUB(void) = 0;

    /**
     * Processes a CodeListing::MUL instruction.
     *
     * @returns \c true if the instruction was successfully processed.
     */
    virtual bool processInstMUL(void) = 0;

    /**
     * Processes a CodeListing::DIV instruction.
     *
     * @returns \c true if the instruction was successfully processed.
     */
    virtual bool processInstDIV(void) = 0;

    /**
     * Processes a CodeListing::SWAP instruction.
     *
     * @returns \c true if the instruction was successfully processed.
     */
    virtual bool processInstSWAP(void) = 0;

    /**
     * Processes a CodeListing::PRINT instruction.
     *
     * @returns \c true if the instruction was successfully processed.
     */
    virtual bool processInstPRINT(void) = 0;

    /**
     * Processes an unknown instruction. Note that returning \c true from this
     * method will resume the execution.
     *
     * @param inst
     *        Byte value of the unknown instruction.
     * @returns \c true if the execution should continue.
     */
    virtual bool processInstUnknown(char inst) = 0;

    /**
     * Gets the program counter value of the instruction that is currently being
     * executed.
     *
     * @returns Program counter value.
     */
    int getPC(void) const;

    /**
     * Gets the program counter value of the last instruction.
     *
     * @returns Program counter value.
     */
    int getPCAtEndOfProgram(void) const;

    /**
     * Gets the size of the program.
     *
     * @returns Program size (in bytes).
     */
    int getProgramSize(void) const;

  private:
    /* TASK: Add whatever private members and methods you find necessary */
};

#endif
