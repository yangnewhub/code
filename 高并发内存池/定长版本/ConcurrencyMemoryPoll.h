#pragma once

#include <iostream>
#include <vector>
#include "Comm.h"
using std::cout;
using std::endl;

#ifdef _WIN32
#include<windows.h>
#else
// 
#endif
inline static void* SystemAlloc(size_t kpage)
{
#ifdef _WIN32
	void* ptr = VirtualAlloc(0, kpage << PAGE_SHIFT, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
#else
	// linux下brk mmap等
#endif

	if (ptr == nullptr)
		throw std::bad_alloc();

	return ptr;
}

inline static void SystemFree(void* ptr)
{
#ifdef _WIN32
	VirtualFree(ptr, 0, MEM_RELEASE);
#else
	// sbrk unmmap等
#endif
}


template <class T>
class ObjectPool
{
public:
	T* New()
	{
		T* obj = nullptr;
		//先判断内存够不够，不够的话再开内存
		//虽然memory不是空，但是申请时内存不够也需要去申请
		//申请时优先在自由链表中获取

		//空
		if (!_freeList)
		{
			if (_memorysize < sizeof(T))
			{
				int size = 128 * 1024;
				//_memory = (void*)malloc(size);
				_memory = SystemAlloc(size >> 13);

				_memorysize = size;
			}

			//内存够了，申请内存
			//必须满足最少开了一个指针的大小
			int memorysize = sizeof(T) < sizeof(void*) ? sizeof(void*) : sizeof(T);

			obj = (T*)_memory;
			_memory = (char*)_memory + memorysize;
			_memorysize -= memorysize;
		}
		else
		{
			obj = (T*)_freeList;
			_freeList = *(void**)_freeList;
		}

		new(obj)T;
		return obj;
	}
	void Delete(T* obj)
	{
		obj->~T();
		//头插
		*(void**)obj = _freeList;
		_freeList = obj;
	}
private:
	void* _memory = nullptr;
	void* _freeList = nullptr;
	int _memorysize = 0;
};

//struct TreeNode
//{
//	int _val;
//	TreeNode* _left;
//	TreeNode* _right;
//
//	TreeNode()
//		:_val(0)
//		, _left(nullptr)
//		, _right(nullptr)
//	{}
//};
//void TestObjectPool()
//{
//	// 申请释放的轮次
//	const size_t Rounds = 5;
//
//	// 每轮申请释放多少次
//	const size_t N = 100000;
//
//	std::vector<TreeNode*> v1;
//	v1.reserve(N);
//
//	size_t begin1 = clock();
//	for (size_t j = 0; j < Rounds; ++j)
//	{
//		for (int i = 0; i < N; ++i)
//		{
//			v1.push_back(new TreeNode);
//		}
//		for (int i = 0; i < N; ++i)
//		{
//			delete v1[i];
//		}
//		v1.clear();
//	}
//
//	size_t end1 = clock();
//
//	std::vector<TreeNode*> v2;
//	v2.reserve(N);
//
//	ObjectPool<TreeNode> TNPool;
//	size_t begin2 = clock();
//	for (size_t j = 0; j < Rounds; ++j)
//	{
//		for (int i = 0; i < N; ++i)
//		{
//			v2.push_back(TNPool.New());
//		}
//		for (int i = 0; i < N; ++i)
//		{
//			TNPool.Delete(v2[i]);
//		}
//		v2.clear();
//	}
//	size_t end2 = clock();
//
//	cout << "new cost time:" << end1 - begin1 << endl;
//	cout << "object pool cost time:" << end2 - begin2 << endl;
//}