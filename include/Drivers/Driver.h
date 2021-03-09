#ifndef SGG_OS_DRIVER_H
#define SGG_OS_DRIVER_H

#include "types.h"

class Driver {
public:
    virtual void activate();

    virtual void deactivate();

    virtual int32_t reset();

protected:
    Driver();

    ~Driver();
};

#endif //SGG_OS_DRIVER_H
