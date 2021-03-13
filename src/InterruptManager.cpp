#include "InterruptManager.h"

InterruptManager::GateDescriptor InterruptManager::interrupt_descriptors[256];
InterruptManager *InterruptManager::active_interrupt_manager = nullptr;

InterruptManager::InterruptManager(uint16_t hardware_interrupt_offset, GlobalDescriptorTable *gdt,
                                   TaskManager *task_manager)
        : pic_master_command(0x20), pic_master_data(0x21),
          pic_slave_command(0xA0), pic_slave_data(0xA1),
          task_manager(task_manager), hardware_interrupt_offset(hardware_interrupt_offset) {
    uint16_t code_segment = gdt->codeSegmentSelector();
    const uint8_t IDT_INTERRUPT_GATE = 0xE;
    for (uint8_t i = 255; i > 0; --i) {
        setInterruptDescriptor(i, code_segment, &ignoreInterrupt, 0, IDT_INTERRUPT_GATE);
        handlers[i] = nullptr;
    }
    setInterruptDescriptor(0, code_segment, &ignoreInterrupt, 0, IDT_INTERRUPT_GATE);
    handlers[0] = nullptr;

    setInterruptDescriptor(0x00, code_segment, &handleException0x00, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptor(0x01, code_segment, &handleException0x01, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptor(0x02, code_segment, &handleException0x02, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptor(0x03, code_segment, &handleException0x03, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptor(0x04, code_segment, &handleException0x04, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptor(0x05, code_segment, &handleException0x05, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptor(0x06, code_segment, &handleException0x06, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptor(0x07, code_segment, &handleException0x07, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptor(0x08, code_segment, &handleException0x08, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptor(0x09, code_segment, &handleException0x09, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptor(0x0A, code_segment, &handleException0x0A, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptor(0x0B, code_segment, &handleException0x0B, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptor(0x0C, code_segment, &handleException0x0C, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptor(0x0D, code_segment, &handleException0x0D, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptor(0x0E, code_segment, &handleException0x0E, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptor(0x0F, code_segment, &handleException0x0F, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptor(0x10, code_segment, &handleException0x10, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptor(0x11, code_segment, &handleException0x11, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptor(0x12, code_segment, &handleException0x12, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptor(0x13, code_segment, &handleException0x13, 0, IDT_INTERRUPT_GATE);

    setInterruptDescriptor(hardware_interrupt_offset + 0x00, code_segment, &handleInterruptRequest0x00, 0,
                           IDT_INTERRUPT_GATE);
    setInterruptDescriptor(hardware_interrupt_offset + 0x01, code_segment, &handleInterruptRequest0x01, 0,
                           IDT_INTERRUPT_GATE);
    setInterruptDescriptor(hardware_interrupt_offset + 0x02, code_segment, &handleInterruptRequest0x02, 0,
                           IDT_INTERRUPT_GATE);
    setInterruptDescriptor(hardware_interrupt_offset + 0x03, code_segment, &handleInterruptRequest0x03, 0,
                           IDT_INTERRUPT_GATE);
    setInterruptDescriptor(hardware_interrupt_offset + 0x04, code_segment, &handleInterruptRequest0x04, 0,
                           IDT_INTERRUPT_GATE);
    setInterruptDescriptor(hardware_interrupt_offset + 0x05, code_segment, &handleInterruptRequest0x05, 0,
                           IDT_INTERRUPT_GATE);
    setInterruptDescriptor(hardware_interrupt_offset + 0x06, code_segment, &handleInterruptRequest0x06, 0,
                           IDT_INTERRUPT_GATE);
    setInterruptDescriptor(hardware_interrupt_offset + 0x07, code_segment, &handleInterruptRequest0x07, 0,
                           IDT_INTERRUPT_GATE);
    setInterruptDescriptor(hardware_interrupt_offset + 0x08, code_segment, &handleInterruptRequest0x08, 0,
                           IDT_INTERRUPT_GATE);
    setInterruptDescriptor(hardware_interrupt_offset + 0x09, code_segment, &handleInterruptRequest0x09, 0,
                           IDT_INTERRUPT_GATE);
    setInterruptDescriptor(hardware_interrupt_offset + 0x0A, code_segment, &handleInterruptRequest0x0A, 0,
                           IDT_INTERRUPT_GATE);
    setInterruptDescriptor(hardware_interrupt_offset + 0x0B, code_segment, &handleInterruptRequest0x0B, 0,
                           IDT_INTERRUPT_GATE);
    setInterruptDescriptor(hardware_interrupt_offset + 0x0C, code_segment, &handleInterruptRequest0x0C, 0,
                           IDT_INTERRUPT_GATE);
    setInterruptDescriptor(hardware_interrupt_offset + 0x0D, code_segment, &handleInterruptRequest0x0D, 0,
                           IDT_INTERRUPT_GATE);
    setInterruptDescriptor(hardware_interrupt_offset + 0x0E, code_segment, &handleInterruptRequest0x0E, 0,
                           IDT_INTERRUPT_GATE);
    setInterruptDescriptor(hardware_interrupt_offset + 0x0F, code_segment, &handleInterruptRequest0x0F, 0,
                           IDT_INTERRUPT_GATE);
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
    if (active_interrupt_manager != nullptr) {
        active_interrupt_manager->deactivate();
    }
    active_interrupt_manager = this;
    asm("sti");
}

void InterruptManager::deactivate() {
    if (active_interrupt_manager == this) {
        active_interrupt_manager = nullptr;
        asm("cli");
    }
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
    if (active_interrupt_manager != nullptr) {
        return active_interrupt_manager->doHandleInterrupt(interrupt_number, stack_ptr);
    }

    return stack_ptr;
}

uint32_t InterruptManager::doHandleInterrupt(uint8_t interrupt_number, uint32_t stack_ptr) {
    if (handlers[interrupt_number] != nullptr) {
        stack_ptr = handlers[interrupt_number]->handle(stack_ptr);
    } else if (interrupt_number != hardware_interrupt_offset) { // IF NOT TIMER EVENT
        printf("Unhandled interrupt: ");
        printfHex8(interrupt_number);
        printf("!\n");
    }

    if (interrupt_number == hardware_interrupt_offset) {
        stack_ptr = (uint32_t)task_manager->schedule((CPUState *) stack_ptr);
    }

    if (hardware_interrupt_offset <= interrupt_number && interrupt_number < 0x30) {
        pic_master_command.write(0x20);
        if (0x28 <= interrupt_number) {
            pic_slave_command.write(0x20);
        }
    }

    return stack_ptr;
}