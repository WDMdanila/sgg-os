#ifndef SGG_OS_PORT_H
#define SGG_OS_PORT_H

#include "Types.h"

class Port {
public:
    explicit Port(uint16_t number);

    virtual ~Port();

protected:
    uint16_t number;
};

#endif //SGG_OS_PORT_H
