#ifndef SGG_OS_MULTITASKING_H
#define SGG_OS_MULTITASKING_H

#include "Types.h"
#include "GDT.h"

struct CPUState {
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;

    uint32_t esi;
    uint32_t edi;
    uint32_t ebp;

//    uint32_t gs;
//    uint32_t fs;
//    uint32_t es;
//    uint32_t ds;

    uint32_t error;

    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
    uint32_t esp;
    uint32_t ss;
} __attribute__((packed));

class Task {
    friend class TaskManager;

public:
    Task(GlobalDescriptorTable *gdt, void entrypoint());

    ~Task();

private:
    CPUState *cpu_state;
    uint8_t stack[4096];
};

class TaskManager {
public:
    TaskManager();

    ~TaskManager();

    bool addTask(Task *task);

    CPUState *schedule(CPUState *cpu_state);

private:
    Task *tasks[256];
    int32_t num_tasks = 0;
    int32_t current_task = -1;
};

#endif //SGG_OS_MULTITASKING_H
