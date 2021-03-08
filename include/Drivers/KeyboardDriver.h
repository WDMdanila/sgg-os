#ifndef SGG_OS_KEYBOARDDRIVER_H
#define SGG_OS_KEYBOARDDRIVER_H

#include "interrupts.h"
#include "Ports/Port.h"
#include "Drivers/InterruptHandler.h"

class KeyboardDriver : public InterruptHandler {
public:
    KeyboardDriver(InterruptManager *manager);

    ~KeyboardDriver();

    virtual uint32_t handle(uint32_t stack_ptr) override;

private:
    Port8Bit data_port;
    Port8Bit command_port;

};

#endif //SGG_OS_KEYBOARDDRIVER_H
