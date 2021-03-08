#ifndef SGG_OS_PORT32BIT_H
#define SGG_OS_PORT32BIT_H

#include "Ports/Port.h"

class Port32Bit : public Port {
public:
    Port32Bit(uint16_t number);

    ~Port32Bit();

    virtual void write(uint32_t data);

    virtual uint32_t read();
};

#endif //SGG_OS_PORT32BIT_H
