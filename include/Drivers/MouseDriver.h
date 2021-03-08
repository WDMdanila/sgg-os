#ifndef SGG_OS_MOUSEDRIVER_H
#define SGG_OS_MOUSEDRIVER_H

#include "Drivers/InterruptHandler.h"
#include "Ports/Port8Bit.h"

class MouseDriver : public InterruptHandler {
public:
    MouseDriver(InterruptManager *manager);

    ~MouseDriver();

    virtual uint32_t handle(uint32_t stack_ptr) override;

    uint8_t buffer[3];
    uint8_t offset;
    uint8_t buttons;
    int8_t x, y;
protected:
    void invertCursorColor() const;

private:
    Port8Bit data_port;
    Port8Bit command_port;

};

#endif //SGG_OS_MOUSEDRIVER_H
