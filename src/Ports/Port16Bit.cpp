#include "Ports/Port16Bit.h"

Port16Bit::Port16Bit(uint16_t number) : Port(number) {}

Port16Bit::~Port16Bit() {}

void Port16Bit::write(uint16_t data) {
    __asm__ volatile("outw %0, %1": :"a"(data), "Nd" (number));
}

uint16_t Port16Bit::read() {
    uint16_t result;
    __asm__ volatile("inw %1, %0": "=a" (result) : "Nd" (number));
    return result;
}
