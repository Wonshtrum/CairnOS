#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "utils/types.h"
#include "utils/io.h"

#define HAS_NEXT 0xF0
#define ALLOCATED 0x0F
#define MIN_CHUNK_SIZE 1


struct Memory_chunk {
	Memory_chunk* link;
	size_t size;
	uint8_t flags;
	void print();
};


class Memory_mamanger {
private:
	Memory_chunk* first;
	static Memory_mamanger* instance;

public:
	Memory_mamanger(size_t start, size_t size);
	~Memory_mamanger();

	void* malloc(size_t size);
	void free(void* ptr);

	void diagnostic();

	static Memory_mamanger* get();
};


void* operator new(size_t size);
void* operator new[](size_t size);

void operator delete(void* ptr);
void operator delete[](void* ptr);

void* malloc(size_t size);


#endif
