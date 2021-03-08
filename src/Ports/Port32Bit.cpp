#include "Ports/Port32Bit.h"

Port32Bit::Port32Bit(uint16_t number) : Port(number) {}

Port32Bit::~Port32Bit() {}

void Port32Bit::write(uint32_t data) {
    __asm__ volatile("outl %0, %1": :"a"(data), "Nd" (number));
}

uint32_t Port32Bit::read() {
    uint32_t result;
    __asm__ volatile("inl %1, %0": "=a" (result) : "Nd" (number));
    return result;
}