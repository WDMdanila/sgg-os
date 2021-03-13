#ifndef SGG_OS_INTERRUPTMANAGER_H
#define SGG_OS_INTERRUPTMANAGER_H

#include "Types.h"
#include "STD.h"
#include "Ports/Port8Bit.h"
#include "GDT.h"
#include "Drivers/InterruptHandler.h"
#include "Multitasking.h"


class InterruptManager {
    friend class InterruptHandler;

protected:
    static InterruptManager *active_interrupt_manager;
    InterruptHandler *handlers[256];
    TaskManager *task_manager;
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
    uint16_t hardware_interrupt_offset;
public:
    InterruptManager(uint16_t hardware_interrupt_offset, GlobalDescriptorTable *gdt, TaskManager *task_manager);

    ~InterruptManager();

    void activate();

    void deactivate();

    static uint32_t handleInterrupt(uint8_t interrupt_number, uint32_t stack_ptr);

    uint32_t doHandleInterrupt(uint8_t interrupt_number, uint32_t stack_ptr);

    static void ignoreInterrupt();

    static void handleInterruptRequest0x00();

    static void handleInterruptRequest0x01();

    static void handleInterruptRequest0x02();

    static void handleInterruptRequest0x03();

    static void handleInterruptRequest0x04();

    static void handleInterruptRequest0x05();

    static void handleInterruptRequest0x06();

    static void handleInterruptRequest0x07();

    static void handleInterruptRequest0x08();

    static void handleInterruptRequest0x09();

    static void handleInterruptRequest0x0A();

    static void handleInterruptRequest0x0B();

    static void handleInterruptRequest0x0C();

    static void handleInterruptRequest0x0D();

    static void handleInterruptRequest0x0E();

    static void handleInterruptRequest0x0F();

    static void handleInterruptRequest0x31();

    static void handleException0x00();

    static void handleException0x01();

    static void handleException0x02();

    static void handleException0x03();

    static void handleException0x04();

    static void handleException0x05();

    static void handleException0x06();

    static void handleException0x07();

    static void handleException0x08();

    static void handleException0x09();

    static void handleException0x0A();

    static void handleException0x0B();

    static void handleException0x0C();

    static void handleException0x0D();

    static void handleException0x0E();

    static void handleException0x0F();

    static void handleException0x10();

    static void handleException0x11();

    static void handleException0x12();

    static void handleException0x13();
};

#endif //SGG_OS_INTERRUPTMANAGER_H
