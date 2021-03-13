#ifndef SGG_OS_PORT16BIT_H
#define SGG_OS_PORT16BIT_H

#include "Ports/Port.h"

class Port16Bit : public Port {
public:
    Port16Bit(uint16_t number);

    ~Port16Bit() override;

    virtual void write(uint16_t data);

    virtual uint16_t read();
};

#endif //SGG_OS_PORT16BIT_H
