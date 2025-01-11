#pragma once

#include <iostream>	
#include <unordered_map>
#include <mutex>
#include "Comm.h"
#include "ConcurrencyMemoryPoll.h"

class PageCache
{
private:
	PageCache() {}
	PageCache(const PageCache&) = delete;
	PageCache& operator=(const PageCache&) = delete;

public:
	static PageCache* GetOneInstance()
	{

		return &_pagecache;
	
	}
	//��Ҫ��page�л�ȡspan
	Span* GetSpanOfPage(int num);
	void ReleseSpanToPageCache(Span* span);
private:
	static PageCache _pagecache;
	SpanList _freelist[PAGE_SIZE];
public:
	static std::mutex _pmux;
	std::unordered_map<TYPE, Span*> _idtospan;

	static ObjectPool<Span> _spanptr;
};										