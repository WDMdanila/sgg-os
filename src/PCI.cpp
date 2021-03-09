#include "STD.h"
#include "PCI.h"
#include "Drivers/DriverManager.h"


PCIDeviceDescriptor::PCIDeviceDescriptor() {}

PCIDeviceDescriptor::~PCIDeviceDescriptor() {}


PCIController::PCIController() : data_port(0xCFC), command_port(0xCF8) {

}

PCIController::~PCIController() {

}

uint32_t PCIController::read(uint16_t bus, uint16_t device, uint16_t function, uint32_t register_offset) {
    uint32_t id = 0x1 << 31
                  | ((bus & 0xFF) << 16)
                  | ((device & 0x1F) << 11)
                  | ((function & 0x07) << 8)
                  | (register_offset & 0xFC);
    command_port.write(id);
    auto result = data_port.read();
    return result >> (8 * (register_offset % 4));
}

void PCIController::write(uint16_t bus, uint16_t device, uint16_t function, uint32_t register_offset,
                          uint32_t value) {
    uint32_t id = 0x1 << 31
                  | ((bus & 0xFF) << 16)
                  | ((device & 0x1F) << 11)
                  | ((function & 0x07) << 8)
                  | (register_offset & 0xFC);
    command_port.write(id);
    data_port.write(value);
}

bool PCIController::deviceHasFunctions(uint16_t bus, uint16_t device) {
    return read(bus, device, 0, 0x0E) & (1 << 7);
}

void PCIController::selectDrivers(DriverManager *driver_manager) {
    for (int32_t bus = 0; bus < 8; bus++) {
        for (int32_t device = 0; device < 32; device++) {
            int32_t functions_num = deviceHasFunctions(bus, device) ? 8 : 1;
            for (int32_t function = 0; function < functions_num; function++) {
                PCIDeviceDescriptor dev = getDeviceDescriptor(bus, device, function);
                if (dev.vendor_id == 0x0000 || dev.vendor_id == 0xFFFF) {
                    continue;
                }
                printf("PCI BUS ");
                printfHex32(bus);
                printf(", DEVICE ");
                printfHex32(device);
                printf(", FUNCTION ");
                printfHex32(function);
                printf(", INTERRUPT ");
                printfHex32(dev.interrupt);
                printf(" = VENDOR ");
                printfHex16(dev.vendor_id);
                printf(", DEVICE ");
                printfHex16(dev.device_id);
                printf("\n");
            }
        }
    }
}

PCIDeviceDescriptor PCIController::getDeviceDescriptor(uint16_t bus, uint16_t device, uint16_t function) {
    PCIDeviceDescriptor result;
    result.bus = bus;
    result.device = device;
    result.function = function;

    result.vendor_id = read(bus, device, function, 0x00);
    result.device_id = read(bus, device, function, 0x02);

    result.class_id = read(bus, device, function, 0x0B);
    result.subclass_id = read(bus, device, function, 0x0A);
    result.interface_id = read(bus, device, function, 0x09);

    result.revision = read(bus, device, function, 0x08);
    result.interrupt = read(bus, device, function, 0x3C);

    return result;
}
