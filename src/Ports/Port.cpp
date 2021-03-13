#include "Ports/Port.h"
#include "STD.h"

Port::Port(uint16_t number) {
    this->number = number;
}

Port::~Port() {
    printf("PORT BASE DESTRUCTED\n");
}
