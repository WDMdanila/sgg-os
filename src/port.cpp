#include "port.h"

Port::Port(uint16_t number) {
    this->number = number;
}

Port::~Port() {}

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

Port8BitSlow::Port8BitSlow(uint16_t number) : Port8Bit(number) {}

Port8BitSlow::~Port8BitSlow() {}

void Port8BitSlow::write(uint8_t data) {
    __asm__ volatile("outb %0, %1\njmp 1f\n1: jmp 1f\n1:": :"a"(data), "Nd" (number));
}