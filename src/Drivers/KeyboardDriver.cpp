#include "Drivers/KeyboardDriver.h"
#include "std.h"

KeyboardDriver::KeyboardDriver(InterruptManager *manager)
        : InterruptHandler(0x21, manager),
          data_port(0x60), command_port(0x64) {}

KeyboardDriver::~KeyboardDriver() {

}

uint32_t KeyboardDriver::handle(uint32_t stack_ptr) {
    // TODO: Add Shift handling
    // TODO: Add different letters handling
    uint8_t key = data_port.read();
    if (key < 0x80) {
        switch (key) {
            case 0xFA:
            case 0x45:
            case 0xC5:
                break;
            default:
                printf("Key pressed: ");
                printfHex8(key);
                printf("\n");
                break;
        }
    }
    return stack_ptr;
}

void KeyboardDriver::activate() {
    while (command_port.read() & 0x1) {
        data_port.read();
    }
    command_port.write(0xAE); // activate interrupts
    command_port.write(0x20); // get current state
    uint8_t status = (data_port.read() | 1) & ~0x10;
    command_port.write(0x60); // prepare for setting state
    data_port.write(status);
    data_port.write(0xF4);
}

void KeyboardDriver::deactivate() {

}

int32_t KeyboardDriver::reset() {
    return 0;
}
