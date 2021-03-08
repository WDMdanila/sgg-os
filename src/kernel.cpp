#include "std.h"
#include "interrupts.h"
#include "Drivers/KeyboardDriver.h"
#include "Drivers/MouseDriver.h"

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

    KeyboardDriver keyboard_driver(&interrupt_manager);
    MouseDriver mouseDriver(&interrupt_manager);

    interrupt_manager.activate();
    while (1);
}