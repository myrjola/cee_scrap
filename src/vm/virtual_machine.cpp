
#include "virtual_machine.hpp"
#include "../io/reporter.hpp"
#include <iostream>
using std::vector;

VirtualMachine::VirtualMachine(void) {
    /* TASK: Add whatever code you find necessary */
}

VirtualMachine::~VirtualMachine(void) {
    /* TASK: Add whatever code you find necessary */
}

void VirtualMachine::execute(const vector<char>& program) {
    /* TASK: Implement this method */
}

bool VirtualMachine::prepareEnvironment(void) {
    /* TASK: Implement this method */
}

bool VirtualMachine::processMagicNumber(int number) {
    /* TASK: Implement this method */
}

bool VirtualMachine::processMemorySize(int value) {
    /* TASK: Implement this method */
}

bool VirtualMachine::processInstLOAD(void) {
    /* TASK: Implement this method */
}

bool VirtualMachine::processInstSTORE(void) {
    /* TASK: Implement this method */
}

bool VirtualMachine::processInstCONST_1B(char value) {
    /* TASK: Implement this method */
}

bool VirtualMachine::processInstCONST_2B(short value) {
    /* TASK: Implement this method */
}

bool VirtualMachine::processInstCONST_4B(int value) {
    /* TASK: Implement this method */
}

bool VirtualMachine::processInstCONST_0(void) {
    /* TASK: Implement this method */
}

bool VirtualMachine::processInstCONST_1(void) {
    /* TASK: Implement this method */
}

bool VirtualMachine::processInstADD(void) {
    /* TASK: Implement this method */
}

bool VirtualMachine::processInstSUB(void) {
    /* TASK: Implement this method */
}

bool VirtualMachine::processInstMUL(void) {
    /* TASK: Implement this method */
}

bool VirtualMachine::processInstDIV(void) {
    /* TASK: Implement this method */
}

bool VirtualMachine::processInstSWAP(void) {
    /* TASK: Implement this method */
}

bool VirtualMachine::processInstPRINT(void) {
    /* TASK: Implement this method */
}

bool VirtualMachine::processInstUnknown(char inst) {
    /* TASK: Implement this method */
}

