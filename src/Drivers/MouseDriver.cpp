#include "Drivers/MouseDriver.h"
#include "std.h"


MouseDriver::MouseDriver(InterruptManager *manager)
        : InterruptHandler(0x2C, manager),
          data_port(0x60), command_port(0x64) {
    x = 40;
    y = 12;
    invertCursorColor();
    offset = 0;
    buttons = 0;
    command_port.write(0xA8); // activate mouse
    command_port.write(0x20); // get current state
    uint8_t status = data_port.read() | 2;
    command_port.write(0x60); // prepare for setting state
    data_port.write(status);
    command_port.write(0xD4);
    data_port.write(0xF4);
    data_port.read();
}

MouseDriver::~MouseDriver() {

}

uint32_t MouseDriver::handle(uint32_t stack_ptr) {
    uint8_t status = command_port.read();
    if (!(status & 0x20))
        return stack_ptr;

    buffer[offset] = data_port.read();
    offset = (offset + 1) % 3;

    if (offset == 0) {
        if (buffer[1] != 0 || buffer[2] != 0) {
            invertCursorColor();

            x += buffer[1];
            if (x >= 80) x = 79;
            if (x < 0) x = 0;
            y -= buffer[2];
            if (y >= 25) y = 24;
            if (y < 0) y = 0;

            invertCursorColor();
        }

        for(uint8_t i = 0; i < 3; i++)
        {
            if((buffer[0] & (0x1<<i)) != (buttons & (0x1<<i)))
            {
                if(buttons & (0x1<<i))
                    printf("Mouse released!");
                else
                    printf("Mouse pressed!");
            }
        }
        buttons = buffer[0];
    }
    return stack_ptr;
}

void MouseDriver::invertCursorColor() const {
    static auto *VideoMemory = (uint16_t *) 0xb8000;
    VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0x0F00) << 4
                              | (VideoMemory[80 * y + x] & 0xF000) >> 4
                              | (VideoMemory[80 * y + x] & 0x00FF);
}
