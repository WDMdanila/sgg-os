#include "Ports/Port8Bit.h"

Port8Bit::Port8Bit(uint16_t number) : Port(number) {}

Port8Bit::~Port8Bit() {}

void Port8Bit::write(uint8_t data) {
    __asm__ volatile("outb %0, %1": :"a"(data), "Nd" (number));
}

uint8_t Port8Bit::read() {
    uint8_t result;
    __asm__ volatile("inb %1, %0": "=a" (result) : "Nd" (number));
    return result;
}

Port8BitSlow::Port8BitSlow(uint16_t number) : Port8Bit(number) {}

Port8BitSlow::~Port8BitSlow() {}

void Port8BitSlow::write(uint8_t data) {
    __asm__ volatile("outb %0, %1\njmp 1f\n1: jmp 1f\n1:": :"a"(data), "Nd" (number));
}
