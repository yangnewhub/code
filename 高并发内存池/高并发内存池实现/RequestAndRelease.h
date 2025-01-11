#pragma once 
#include "ThreadCache.h"
#include "Comm.h"
#include "PageCache.h"

static void* AllocMemory(int size)
{
	assert(size > 0);

	if (size > MAX_BYTES)
	{
		int alignum = SizeNum::AlignNum(size);
		int n = SizeNum::NumOfPage(alignum);

		PageCache::GetOneInstance()->_pmux.lock();

		Span*span = PageCache::GetOneInstance()->GetSpanOfPage(size);
		span->_isUse = true;
		span->_size = size;

		PageCache::GetOneInstance()->_idtospan[span->_pageid] = span;

		PageCache::GetOneInstance()->_pmux.unlock();

		return span->_freelist;

	}
	else 
	{
		if (threadCache == nullptr)
		{
			threadCache = new ThreadCache;
		}
	}

	return threadCache->New(size);
}

static void DeleteMemory(void* ptr)
{
	assert(ptr);

	PageCache::GetOneInstance()->_pmux.lock();

	Span* span = PageCache::GetOneInstance()->_idtospan[SizeNum::PageId(ptr)];

	
	PageCache::GetOneInstance()->_pmux.unlock();

	TYPE size = span->_size;

	if (size > MAX_BYTES)
	{
		PageCache::GetOneInstance()->_pmux.lock();


		PageCache::GetOneInstance()->ReleseSpanToPageCache(span);
		
		PageCache::GetOneInstance()->_pmux.unlock();
	}
	else
	{
		threadCache->Delete(ptr, size);
	}

}