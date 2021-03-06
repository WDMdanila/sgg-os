#include "std.h"
#include "interrupts.h"

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
    printf("Hello 1\tlmao\n");
    GlobalDescriptorTable gdt;
    printf("Hello 2\tlmao\n");
    InterruptManager interrupt_manager(&gdt);
    printf("Hello 3\tlmao\n");
    interrupt_manager.activate();
    printf("Hello 4\tlmao\n");
    while (1);
}