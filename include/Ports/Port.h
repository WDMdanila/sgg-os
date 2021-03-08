#ifndef SGG_OS_PORT_H
#define SGG_OS_PORT_H

#include "types.h"

class Port {
protected:
    uint16_t number;

    Port(uint16_t number);

    ~Port();
};

#endif //SGG_OS_PORT_H
