#include "Drivers/InterruptHandler.h"
#include "interrupts.h"

InterruptHandler::InterruptHandler(uint8_t interrupt_num, InterruptManager *manager)
        : interrupt_num(interrupt_num), manager(manager) {
    manager->handlers[interrupt_num] = this;
}

InterruptHandler::~InterruptHandler() {
    if (manager->handlers[interrupt_num] == this) {
        manager->handlers[interrupt_num] = nullptr;
    }
}

uint32_t InterruptHandler::handle(uint32_t stack_ptr) {
    return stack_ptr;
}