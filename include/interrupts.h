#ifndef SGG_OS_INTERRUPTS_H
#define SGG_OS_INTERRUPTS_H

#include "types.h"
#include "port.h"
#include "gdt.h"

class InterruptManager {
protected:
    struct GateDescriptor {
        uint16_t address_low_bits;
        uint16_t gdt_code_segment_selector_offset;
        uint8_t reserved;
        uint8_t access;
        uint16_t address_high_bits;
    } __attribute__((packed));

    static GateDescriptor interrupt_descriptors[256];
    struct InterruptDescriptorTablePointer {
        uint16_t size;
        uint32_t base;
    } __attribute__((packed));

    static void setInterruptDescriptor(uint8_t interrupt_num,
                                       uint16_t gdt_code_segment_selector_offset,
                                       void (*handler)(),
                                       uint8_t access,
                                       uint8_t type);

    Port8BitSlow pic_master_command;
    Port8BitSlow pic_master_data;
    Port8BitSlow pic_slave_command;
    Port8BitSlow pic_slave_data;
public:
    InterruptManager(GlobalDescriptorTable *gdt);

    ~InterruptManager();

    void activate();

    static uint32_t handleInterrupt(uint8_t interrupt_number, uint32_t stack_ptr);

    static void ignoreInterrupt();

    static void handleInterruptRequest0x00();

    static void handleInterruptRequest0x01();
};

#endif //SGG_OS_INTERRUPTS_H