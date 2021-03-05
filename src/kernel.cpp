#include "interrupts.h"

#define WIDTH 80
#define HEIGHT 25

void printf(const char *str) {
    static auto *video_memory = (uint16_t *) 0xB8000;
    static uint8_t x = 0, y = 0;
    for (int32_t i = 0; str[i] != '\0'; ++i) {
        switch (str[i]) {
            case '\n':
                y++;
                x = 0;
                break;
            case '\t':
                x += 4 - x % 4;
                break;
            default:
                video_memory[WIDTH * y + x] = (video_memory[WIDTH * y + x] & 0xFF00) | str[i];
                x++;
                break;
        }
        if (x >= WIDTH) {
            y++;
            x = 0;
        }
        if (y >= HEIGHT) {
            for (y = 0; y < HEIGHT; y++) {
                for (x = 0; x < WIDTH; x++) {
                    video_memory[WIDTH * y + x] = (video_memory[WIDTH * y + x] & 0xFF00) | ' ';
                }
            }
            y = 0;
            x = 0;
        }
    }
}

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