#pragma once

#include <iostream>
#include <vector>
#include "Comm.h"

class ThreadCache
{
public:
	void* New(size_t size);
	void Delete(void* ptr, int size);
private:
	FreeList _freelist[THREAD_CANCHE_NUM];
};
static __declspec(thread) ThreadCache* threadCache = nullptr;