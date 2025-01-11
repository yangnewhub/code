#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <map>
#include <algorithm>

#include <time.h>
#include <assert.h>

#include <thread>
#include <mutex>

#ifdef _WIN32
	#include <Windows.h>
#else

#endif 

#ifdef _WIN64
typedef unsigned long long TYPE;
#else
typedef size_t TYPE;
#endif

#define MAXSIZE 1024
#define THREAD_CANCHE_NUM 208
#define PAGE_SHIFT 13
#define PAGE_SIZE 129
#define MAX_BYTES 256*1024


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

static void*& NextObj(void* obj)
{
	return *(void**)obj;
}


static void*& NextPtr(void* obj)
{
	return *(void**)obj;
}
class FreeList
{
public:
	void Push(void * obj)
	{
		NextPtr(obj) = _freelist;
		_freelist = obj;
		_size++;
	}
	void* Pop()
	{
		void* obj = _freelist;
		_freelist = NextPtr(_freelist);
		_size--;
		return obj;
	}
	void PushForRange(void* begin, void* end, int n)
	{
		NextPtr(end) = _freelist;
		_freelist = begin;
		_size += n;
	}
	bool Empty()
	{
		return _freelist == nullptr;
	}
	int& MaxSize()
	{
		return _maxsize;
	}
	int& Size()
	{
		return _size;
	}
	void*& FreeListPtr()
	{
		return _freelist;
	}
private:
	void* _freelist = nullptr;
	int _size = 0;
	int _maxsize = 1;
};
class Span;

class SizeNum
{
public:
	static int NumOfPage(int alignum)
	{
		size_t num = Num(alignum);
		size_t npage = num * alignum;

		npage >>= PAGE_SHIFT;
		if (npage == 0)
			npage = 1;

		return npage;
	}



	static inline size_t _AlignNum(size_t bytes, size_t alignNum)
	{
		return ((bytes + alignNum - 1) & ~(alignNum - 1));
	}

	static inline size_t AlignNum(size_t size)
	{
		if (size <= 128)
		{
			return _AlignNum(size, 8);
		}
		else if (size <= 1024)
		{
			return _AlignNum(size, 16);
		}
		else if (size <= 8 * 1024)
		{
			return _AlignNum(size, 128);
		}
		else if (size <= 64 * 1024)
		{
			return _AlignNum(size, 1024);
		}
		else if (size <= 256 * 1024)
		{
			return _AlignNum(size, 8 * 1024);
		}
		else
		{
			return _AlignNum(size, 1 << PAGE_SHIFT);
		}
	}

	static inline size_t _Index(size_t bytes, size_t align_shift)
	{
		return ((bytes + (1 << align_shift) - 1) >> align_shift) - 1;
	}

	// 计算映射的哪一个自由链表桶
	static inline size_t Index(size_t bytes)
	{
		//assert(bytes <= MAX_BYTES);
		assert(bytes > 0);

		// 每个区间有多少个链
		static int group_array[4] = { 16, 56, 56, 56 };
		if (bytes <= 128) {
			return _Index(bytes, 3);
		}
		else if (bytes <= 1024) {
			return _Index(bytes - 128, 4) + group_array[0];
		}
		else if (bytes <= 8 * 1024) {
			return _Index(bytes - 1024, 7) + group_array[1] + group_array[0];
		}
		else if (bytes <= 64 * 1024) {
			return _Index(bytes - 8 * 1024, 10) + group_array[2] + group_array[1] + group_array[0];
		}
		else if (bytes <= 256 * 1024) {
			return _Index(bytes - 64 * 1024, 13) + group_array[3] + group_array[2] + group_array[1] + group_array[0];
		}
		else {
			return bytes;
		}

		return -1;
	}
	static inline TYPE PageEndPtr(void* ptr)
	{
		return ((((TYPE)ptr) >> PAGE_SHIFT) + 1) << PAGE_SHIFT;
	}

	static int Num(int aligum)
	{
		int num = MAXSIZE / aligum;
		if (num == 0)
			return 2;
		if (num <= 512) return num;
		else return 512;
	}

	static TYPE PageId(void* span)
	{
		return (TYPE)(span) >> PAGE_SHIFT;
	}
};

class Span
{
public:
	//bool _isnew = true;
	TYPE _pageid;//页号
	int _pagecount;//span有几页

	//双向链表管理起来
	Span* _prev = nullptr;
	Span* _next = nullptr;

	int _count = 0;//有几个没有还回来。

	void* _freelist = nullptr;//管理小的freelist
	
	bool _isUse = false;
	TYPE _size;
};



class SpanList
{
public:
	SpanList()
	{
		_head = new Span;
		_head->_prev = _head;
		_head->_next = _head;
	}
	void Insert(Span* pos, Span* span)
	{
		Span* next = pos->_next;
		
		span->_prev = pos;
		span->_next = next;
		pos->_next = span;
		next->_prev = span;
	}
	Span* Erase()
	{
		Span* pos = Begin();
		Span* prev = _head;
		Span* next = pos->_next;

		pos->_prev = nullptr;
		pos->_next = nullptr;
		prev->_next = next;
		next->_prev = prev;
		return pos;
	}
	Span* Begin()
	{
		return _head->_next;
	}
	Span* End()
	{
		return _head;
	}
	bool Empty()
	{
		return Begin() == End();
	}
public:
	Span* _head;
	std::mutex _mux;
};