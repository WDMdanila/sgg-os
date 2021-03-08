#ifndef SGG_OS_PORT8BIT_H
#define SGG_OS_PORT8BIT_H

#include "Port.h"

class Port8Bit : public Port {
public:
    Port8Bit(uint16_t number);

    ~Port8Bit();

    virtual void write(uint8_t data);

    virtual uint8_t read();
};

class Port8BitSlow : public Port8Bit {
public:
    Port8BitSlow(uint16_t number);

    ~Port8BitSlow();

    virtual void write(uint8_t data);
};

#endif //SGG_OS_PORT8BIT_H
