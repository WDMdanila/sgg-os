#ifndef SGG_OS_DRIVERMANAGER_H
#define SGG_OS_DRIVERMANAGER_H

#include "Drivers/Driver.h"
#include "Types.h"

class DriverManager {
public:
    DriverManager();

    ~DriverManager();

    void addDriver(Driver *driver);

    void activateDrivers();

protected:
    uint32_t drivers_num = 0;
    Driver *drivers[256];
};

#endif //SGG_OS_DRIVERMANAGER_H
