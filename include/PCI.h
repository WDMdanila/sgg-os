#ifndef SGG_OS_PCI_H
#define SGG_OS_PCI_H

#include <Drivers/Driver.h>
#include "Ports/Port32Bit.h"
#include "Types.h"
#include "InterruptManager.h"

enum RegisterType {
    UNDEFINED = -1,
    MEMORY_MAPPING = 0,
    INPUT_OUTPUT = 1
};

struct BaseAddressRegister {
    bool prefetchable = false;
    uint8_t *address = nullptr;
    uint32_t size = 0;
    RegisterType type = UNDEFINED;
};

struct PCIDeviceDescriptor {
    uint32_t port_base;
    uint32_t interrupt;

    uint16_t bus;
    uint16_t device;
    uint16_t function;

    uint16_t vendor_id;
    uint16_t device_id;

    uint8_t class_id;
    uint8_t subclass_id;
    uint8_t interface_id;

    uint8_t revision;

    PCIDeviceDescriptor();

    ~PCIDeviceDescriptor();
};

class DriverManager;

class PCIController {
public:
    PCIController();

    ~PCIController();

    uint32_t read(uint16_t bus, uint16_t device, uint16_t function, uint32_t register_offset);

    void write(uint16_t bus, uint16_t device, uint16_t function, uint32_t register_offset, uint32_t value);

    bool deviceHasFunctions(uint16_t bus, uint16_t device);

    Driver *getDriver(const PCIDeviceDescriptor& descriptor, InterruptManager *interrupt_manager);

    void selectDrivers(DriverManager *driver_manager, InterruptManager *interrupt_manager);

    PCIDeviceDescriptor getDeviceDescriptor(uint16_t bus, uint16_t device, uint16_t function);

    BaseAddressRegister getBaseAddressRegister(uint16_t bus, uint16_t device, uint16_t function, uint16_t bar_num);

protected:
    Port32Bit data_port;
    Port32Bit command_port;
};

#endif //SGG_OS_PCI_H
