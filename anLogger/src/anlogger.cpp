#include "anlogger.h"

char anStdErrBuffer[BUFSIZ];

const anTxtAttribType &anOriginalConsoleTextAttribute = [](){
    anTxtAttribType tmp = anDefaultTextAttribute;
    _anGetConsoleTextAttribute(tmp);
    return tmp;
}();

const std::chrono::steady_clock::time_point &anThisProgramStartingTimePoint = [](){
    setvbuf(stderr, anStdErrBuffer, _IOFBF, BUFSIZ);
    (void) anOriginalConsoleTextAttribute;
    return std::chrono::steady_clock::now();
}();
