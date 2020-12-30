#ifndef __MULTITASKING_H__
#define __MULTITASKING_H__

#include "utils/types.h"
#include "utils/io.h"
#include "hardware/gdt.h"

#define MAX_NUM_TASKS 256
#define STACK_SIZE 4096

struct CPU_state {
	uint32_t eax;
	uint32_t ebx;
	uint32_t ecx;
	uint32_t edx;

	uint32_t esi;
	uint32_t edi;
	uint32_t ebp;
	/*
	uint32_t gs;
	uint32_t fs;
	uint32_t es;
	uint32_t ds;
	*/
	uint32_t error;

	uint32_t eip;
	uint32_t cs;
	uint32_t eflags;
	uint32_t esp;
	uint32_t ss;
} __attribute__((packed));


class Task {
friend class Task_manager;
private:
	uint8_t stack[STACK_SIZE];
	CPU_state* cpu_state;

public:
	Task(Global_descriptor_table* gdt, void entrypoint());
	~Task();
};


class Task_manager {
private:
	Task* tasks[MAX_NUM_TASKS];
	int32_t num_tasks;
	int32_t current_task;

public:
	Task_manager();
	~Task_manager();

	bool add_task(Task* task);
	CPU_state* shedule(CPU_state* cpu_state);
};


#endif
