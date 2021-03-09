#ifndef SGG_OS_KEYBOARDDRIVER_H
#define SGG_OS_KEYBOARDDRIVER_H

#include "InterruptManager.h"
#include "Ports/Port.h"
#include "Drivers/InterruptHandler.h"
#include "Drivers/Driver.h"

class KeyboardDriver : public InterruptHandler, public Driver {
public:
    KeyboardDriver(InterruptManager *manager);

    ~KeyboardDriver();

    virtual uint32_t handle(uint32_t stack_ptr) override;

    void activate() override;

    void deactivate() override;

    int32_t reset() override;

private:
    Port8Bit data_port;
    Port8Bit command_port;

};

#endif //SGG_OS_KEYBOARDDRIVER_H
