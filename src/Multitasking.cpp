#include "Multitasking.h"

Task::Task(GlobalDescriptorTable *gdt, void (*entrypoint)()) {
    cpu_state = (CPUState *) (stack + 4096 - sizeof(CPUState));
    cpu_state->eax = 0;
    cpu_state->ebx = 0;
    cpu_state->ecx = 0;
    cpu_state->edx = 0;

    cpu_state->esi = 0;
    cpu_state->edi = 0;
    cpu_state->ebp = 0;

//    cpu_state->esp = 0;
    cpu_state->eip = (uint32_t) entrypoint;
    cpu_state->cs = gdt->codeSegmentSelector();
//    cpu_state->ss = 0;
    cpu_state->eflags = 0x202;

//    cpu_state->gs = 0;
//    cpu_state->fs = 0;
//    cpu_state->es = 0;
//    cpu_state->ds = 0;
//    cpu_state->error = 0;

}

Task::~Task() {}

TaskManager::TaskManager() {}

TaskManager::~TaskManager() {}

bool TaskManager::addTask(Task *task) {
    if (num_tasks >= 256) {
        return false;
    } else {
        tasks[num_tasks] = task;
        num_tasks++;
        return true;
    }
}

CPUState *TaskManager::schedule(CPUState *cpu_state) {
    if (num_tasks <= 0) {
        return cpu_state;
    }
    if (current_task >= 0) {
        tasks[current_task]->cpu_state = cpu_state;
    }
    if (++current_task >= num_tasks) {
        current_task %= num_tasks;
    }
    return tasks[current_task]->cpu_state;
}