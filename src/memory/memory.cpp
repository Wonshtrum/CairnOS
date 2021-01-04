#include "memory.h"


void Memory_chunk::print() {
	print_hex((size_t)this);
	print_str(" ");
	print_hex((size_t)link);
	print_str(" ");
	print_hex((uint8_t)flags);
	print_str(" ");
	print_hex((size_t)size);
	print_str("\n");
}

Memory_mamanger* Memory_mamanger::instance;
Memory_mamanger::Memory_mamanger(size_t start, size_t size) {
	instance = this;
	first = (Memory_chunk*)start;
	first->link = 0;
	first->flags = 0x00;
	first->size = size;
}
Memory_mamanger::~Memory_mamanger() {}

Memory_mamanger* Memory_mamanger::get() {
	return instance;
}

void* Memory_mamanger::malloc(size_t size) {
	Memory_chunk* result = 0;
	Memory_chunk* chunk = first;
	while (true) {
		if (!(chunk->flags & ALLOCATED) && chunk->size >= size) {
			result = chunk;
			break;
		}
		if (chunk->flags & HAS_NEXT) {
			chunk = (Memory_chunk*)((size_t)chunk + chunk->size + sizeof(Memory_chunk));
			continue;
		}
		break;
	}

	if (result == 0) {	// not enough memory
		return 0;
	}

	if (result->size >= sizeof(Memory_chunk) + size + MIN_CHUNK_SIZE) {	// chunk split
		Memory_chunk* new_chunk = (Memory_chunk*)((size_t)result + sizeof(Memory_chunk) + size);
		new_chunk->link = result;
		new_chunk->flags = result->flags;				// inherit HAS_NEXT
		new_chunk->size = result->size - size - sizeof(Memory_chunk);
		result->size = size;
		result->flags = HAS_NEXT;
	}
	result->flags |= ALLOCATED;

	return (void*)((size_t)result + sizeof(Memory_chunk));
}

void Memory_mamanger::free(void* ptr) {
	Memory_chunk* chunk = (Memory_chunk*)((size_t)ptr - sizeof(Memory_chunk));
	size_t size = chunk->size;
	uint8_t flags = chunk->flags & HAS_NEXT;				// deallocate
	if (chunk->link && !(chunk->link->flags & ALLOCATED)) {
		size += sizeof(Memory_chunk) + chunk->link->size;		// grows backwards
		chunk = chunk->link;
	}
	if (flags & HAS_NEXT) {
		Memory_chunk* next_chunk = (Memory_chunk*)((size_t)chunk + sizeof(Memory_chunk) + size);
		if (!(next_chunk->flags & ALLOCATED)) {
			size += sizeof(Memory_chunk) + next_chunk->size;	// grows forwards
			flags &= next_chunk->flags;				// takes new HAS_NEXT
			if (flags & HAS_NEXT) {
				next_chunk = (Memory_chunk*)((size_t)chunk + sizeof(Memory_chunk) + size);
			}
		}
		next_chunk->link = chunk;
	}
	chunk->size = size;
	chunk->flags = flags;
}

void Memory_mamanger::diagnostic() {
	Memory_chunk* chunk = first;
	for (; chunk->flags & HAS_NEXT ; chunk = (Memory_chunk*)((size_t)chunk + chunk->size + sizeof(Memory_chunk))) {
		chunk->print();
	}
	chunk->print();
}

void* operator new(size_t size) {
	return Memory_mamanger::get()->malloc(size);
}
void* operator new[](size_t size) {
	return Memory_mamanger::get()->malloc(size);
}

void operator delete(void* ptr) {
	return Memory_mamanger::get()->free(ptr);
}

void operator delete[](void* ptr) {
	return Memory_mamanger::get()->free(ptr);
}

void* malloc(size_t size) {
	return Memory_mamanger::get()->malloc(size);
}

void free(void* ptr) {
	return Memory_mamanger::get()->free(ptr);
}
