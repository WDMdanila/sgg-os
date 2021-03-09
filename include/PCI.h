#ifndef SGG_OS_PCI_H
#define SGG_OS_PCI_H

#include "Ports/Port32Bit.h"
#include "Types.h"

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

    void selectDrivers(DriverManager *driver_manager);

    PCIDeviceDescriptor getDeviceDescriptor(uint16_t bus, uint16_t device, uint16_t function);

protected:
    Port32Bit data_port;
    Port32Bit command_port;
};

#endif //SGG_OS_PCI_H
