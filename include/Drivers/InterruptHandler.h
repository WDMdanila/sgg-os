#ifndef SGG_OS_INTERRUPTHANDLER_H
#define SGG_OS_INTERRUPTHANDLER_H

#include "Types.h"

class InterruptManager;

class InterruptHandler {
public:
    virtual uint32_t handle(uint32_t stack_ptr);

protected:
    InterruptHandler(uint8_t interrupt_num, InterruptManager *manager);

    ~InterruptHandler();

    uint8_t interrupt_num;
    InterruptManager *manager;
};


#endif //SGG_OS_INTERRUPTHANDLER_H
