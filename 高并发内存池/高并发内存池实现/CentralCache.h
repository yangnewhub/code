#pragma once

#include <iostream>
#include <mutex>
#include "Comm.h"

//���߳���centralcache�л�ȡ�ڴ棬����Ϊ����ģʽ������Ҫ������
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