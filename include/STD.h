#ifndef SGG_OS_STD_H
#define SGG_OS_STD_H

#include "Types.h"

void printf(const char *str);

void printfHex8(uint8_t data);

void printfHex16(uint16_t data);

void printfHex32(uint32_t data);

void clearConsole();

#endif //SGG_OS_STD_H
