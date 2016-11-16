
#include "decoder.hpp"
#include "../generator/code_listing.hpp"
#include "../io/reporter.hpp"

Decoder::Decoder(void) {
    /* TASK: Add whatever code you find necessary */
}

Decoder::~Decoder(void) {
    /* TASK: Add whatever code you find necessary */
}

void Decoder::invoke(const vector<char>& program) {
    /* TASK: Implement this method */
}

bool Decoder::beforeCodeExecution(void) {
    return true;
}

bool Decoder::afterCodeExecution(void) {
    return true;
}


int Decoder::getPC(void) const {
    /* TASK: Implement this method */
}

int Decoder::getPCAtEndOfProgram(void) const {
    /* TASK: Implement this method */
}

int Decoder::getProgramSize(void) const {
    /* TASK: Implement this method */
}

