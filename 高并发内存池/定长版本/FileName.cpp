#include <iostream>
#include "ConcurrencyMemoryPoll.h"

int main()
{
	TestObjectPool();
	std::cout << sizeof(void*) << std::endl;
	return 0;
}