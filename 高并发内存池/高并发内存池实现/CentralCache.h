#pragma once

#include <iostream>
#include <mutex>
#include "Comm.h"

//多线程向centralcache中获取内存，设置为单例模式，还需要加锁。
class CentralCache
{
private:
	CentralCache()
	{
	}
	CentralCache(const CentralCache&) = delete;
	CentralCache& operator=(const CentralCache&) = delete;

public:
	int GetMemoryOfCentralCache(void*& begin, void*& end, int alignum, int index, int num);
	Span* GetNoEmpty(SpanList& list, int alignum, int index);
	static CentralCache* GetOneInstance()
	{
		
		return &_centralcache;

	}
	void ReleselistToCentralCache(void* begin, void* end, int n, int index);
private:
	static CentralCache _centralcache;
	SpanList _spanlist[THREAD_CANCHE_NUM];
	
};