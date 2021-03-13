#include "STD.h"
#include "InterruptManager.h"
#include "Drivers/DriverManager.h"
#include "Drivers/KeyboardDriver.h"
#include "Drivers/MouseDriver.h"
#include "PCI.h"
#include "Multitasking.h"
#include "MemoryManagement.h"

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
extern "C" void __cxa_pure_virtual() {
    // Do nothing or print an error message.
}

void taskA() {
    while (true) {
        printf("Task A!");
    }
}

void taskB() {
    while (true) {
        printf("Task B!");
    }
}

extern "C" void kernelMain(void *multiboot_struct, uint32_t magic_number) {
    clearConsole();
    printf("Hello World!\n");
    GlobalDescriptorTable gdt;
    size_t heap = 10 * 1024 * 1024;
    auto *mem_upper = (uint32_t *) (((size_t) multiboot_struct) + 8);
    MemoryManager memory_manager(heap, (*mem_upper) * 1024 - heap - 10 * 1024);
    TaskManager task_manager;
    Task task_a(&gdt, taskA);
    Task task_b(&gdt, taskB);
//    task_manager.addTask(&task_a);
//    task_manager.addTask(&task_b);
    InterruptManager interrupt_manager(0x20, &gdt, &task_manager);
    DriverManager driver_manager;
    PCIController pci_controller;
    MouseDriver mouse_driver(&interrupt_manager);
    KeyboardDriver keyboard_driver(&interrupt_manager);

    driver_manager.addDriver(&mouse_driver);
    driver_manager.addDriver(&keyboard_driver);

    pci_controller.selectDrivers(&driver_manager, &interrupt_manager);
    driver_manager.activateDrivers();
    interrupt_manager.activate();
    while (1);
}