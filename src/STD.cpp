#include "STD.h"

#define WIDTH 80
#define HEIGHT 25

static auto *video_memory = (uint16_t *) 0xB8000;
static uint8_t x = 0, y = 0;

void printf(const char *str) {
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
            clearConsole();
        }
    }
}

void printfHex8(uint8_t key) {
    char *foo = "00";
    char *hex = "0123456789ABCDEF";
    foo[0] = hex[(key >> 4) & 0xF];
    foo[1] = hex[key & 0xF];
    printf(foo);
}

void printfHex16(uint16_t key) {
    printfHex8((key >> 8) & 0xFF);
    printfHex8(key & 0xFF);
}

void printfHex32(uint32_t key) {
    printfHex8((key >> 24) & 0xFF);
    printfHex8((key >> 16) & 0xFF);
    printfHex8((key >> 8) & 0xFF);
    printfHex8(key & 0xFF);
}

void clearConsole() {
    for (y = 0; y < HEIGHT; y++) {
        for (x = 0; x < WIDTH; x++) {
            video_memory[WIDTH * y + x] = (video_memory[WIDTH * y + x] & 0xFF00) | ' ';
        }
    }
    y = 0;
    x = 0;
}
