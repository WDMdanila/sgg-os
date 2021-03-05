#include "interrupts.h"

void printf(const char *str);

InterruptManager::GateDescriptor InterruptManager::interrupt_descriptors[256];

InterruptManager::InterruptManager(GlobalDescriptorTable *gdt)
        : pic_master_command(0x20), pic_master_data(0x21), pic_slave_command(0xA0), pic_slave_data(0xA1) {
    uint16_t code_segment = gdt->codeSegmentSelector();
    const uint8_t IDT_INTERRUPT_GATE = 0xE;

    for (uint16_t i = 0; i < 256; i++) {
        setInterruptDescriptor(i, code_segment, &ignoreInterrupt, 0, IDT_INTERRUPT_GATE);
    }
    setInterruptDescriptor(0x20, code_segment, &handleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptor(0x21, code_segment, &handleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);

    pic_master_command.write(0x11);
    pic_slave_command.write(0x11);

    pic_master_data.write(0x20);
    pic_slave_data.write(0x28);

    pic_master_data.write(0x04);
    pic_slave_data.write(0x02);

    pic_master_data.write(0x01);
    pic_slave_data.write(0x01);

    pic_master_data.write(0x00);
    pic_slave_data.write(0x00);

    InterruptDescriptorTablePointer idt;
    idt.size = 256 * sizeof(GateDescriptor) - 1;
    idt.base = (uint32_t) interrupt_descriptors;
    asm volatile("lidt %0": : "m" (idt));
}

InterruptManager::~InterruptManager() {}

void InterruptManager::activate() {
    asm("sti");
}

void InterruptManager::setInterruptDescriptor(uint8_t number, uint16_t gdt_code_segment_selector_offset,
                                              void (*handler)(), uint8_t access, uint8_t type) {
    const uint8_t IDT_DESC_PRESENT = 0x80;
    interrupt_descriptors[number].address_low_bits = ((uint32_t) handler) & 0xFFFF;
    interrupt_descriptors[number].address_high_bits = (((uint32_t) handler) >> 16) & 0xFFFF;
    interrupt_descriptors[number].gdt_code_segment_selector_offset = gdt_code_segment_selector_offset;
    interrupt_descriptors[number].access = IDT_DESC_PRESENT | type | ((access & 3) << 5);
    interrupt_descriptors[number].reserved = 0;
}

uint32_t InterruptManager::handleInterrupt(uint8_t interrupt_number, uint32_t stack_ptr) {
    printf("INTERRUPT");
    return stack_ptr;
}