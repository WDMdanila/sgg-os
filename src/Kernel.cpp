#include "STD.h"
#include "InterruptManager.h"
#include "Drivers/DriverManager.h"
#include "Drivers/KeyboardDriver.h"
#include "Drivers/MouseDriver.h"
#include "PCI.h"

typedef void (*constructor)();

extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void execConstructors() {
    printf("Executing constructors...\n");
    for (constructor *i = &start_ctors; i != &end_ctors; i++) {
        (*i)();
    }
    printf("Constructors executed!\n");
}

extern "C" void kernelMain(void *multiboot_struct, uint32_t magic_number) {
    clearConsole();
    printf("Hello World!\n");
    GlobalDescriptorTable gdt;
    InterruptManager interrupt_manager(&gdt);
    DriverManager driver_manager;
    PCIController pci_controller;
    MouseDriver mouse_driver(&interrupt_manager);
    KeyboardDriver keyboard_driver(&interrupt_manager);

    driver_manager.addDriver(&mouse_driver);
    driver_manager.addDriver(&keyboard_driver);

    driver_manager.activateDrivers();
    interrupt_manager.activate();
    pci_controller.selectDrivers(&driver_manager);
    while (1);
}