#include "multitasking.h"


Task::Task(Global_descriptor_table* gdt, void entrypoint()) {
	cpu_state = (CPU_state*)(stack + STACK_SIZE - sizeof(CPU_state));
	cpu_state->eax = 0;
	cpu_state->ebx = 0;
	cpu_state->ecx = 0;
	cpu_state->edx = 0;

	cpu_state->esi = 0;
	cpu_state->edi = 0;
	cpu_state->ebp = 0;
	/*	
	cpu_state->gs = 0;
	cpu_state->fs = 0;
	cpu_state->es = 0;
	cpu_state->ds = 0;
	*/
	//cpu_state->error = 0;
	
	//cpu_state->esp = ;
	cpu_state->eip = (uint32_t)entrypoint;
	cpu_state->cs = gdt->get_code_segment_selector();
	//cpu_state->ss = ;
	cpu_state->eflags = 0x202;
}
Task::~Task() {}


Task_manager::Task_manager(): num_tasks(0), current_task(-1) {}
Task_manager::~Task_manager() {}

bool Task_manager::add_task(Task* task) {
	if (num_tasks >= MAX_NUM_TASKS) {
		return false;
	}
	tasks[num_tasks++] = task;
	return true;
}

CPU_state* Task_manager::shedule(CPU_state* cpu_state) {
	if (num_tasks <= 0) {
		return cpu_state;
	}
	if (current_task >= 0) {
		tasks[current_task]->cpu_state = cpu_state;
	}
	current_task = (current_task + 1) % num_tasks;
	return tasks[current_task]->cpu_state;
}
