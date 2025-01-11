#include <iostream>
#include "PageCache.h"
#include "Comm.h"


ObjectPool<Span> PageCache::_spanptr;
PageCache PageCache::_pagecache;
std::mutex PageCache::_pmux;
Span* PageCache::GetSpanOfPage(int num)
{
	if (num > MAX_BYTES)
	{
		Span* span = _spanptr.New();
		span->_freelist = SystemAlloc(num >> PAGE_SHIFT);
		span->_pagecount = num >> PAGE_SHIFT;
		span->_pageid = SizeNum::PageId(span->_freelist);
		return span;
	}
	else
	{
		//不为空时，直接将页返回5
		if (_freelist[num].Begin() != _freelist[num].End())
		{
			return _freelist[num].Erase();
		}
		// 此时为空
		//向下看，下面有，进行切割将一个整页切割成两份，然后插入
		//下面没有就去申请内存，需要将申请的内存分成一个小的freelist
		for (int i = num; i < PAGE_SIZE; i++)
		{
			if (!_freelist[i].Empty())
			{
				//切割
				Span* span = _freelist[i].Erase();
				//span的尾页加入
				_idtospan[span->_pageid + num - 1] = span;

				int n = i - num;
				//计算nspan
				Span* nspan = _spanptr.New();

				nspan->_freelist = (Span*)((char*)span->_freelist + (num << PAGE_SHIFT));

				span->_pagecount = num;
				nspan->_pagecount = n;
				nspan->_pageid = span->_pageid + num;

				//nspan的首尾页加入
				TYPE i = nspan->_pageid, j = nspan->_pageid + nspan->_pagecount - 1;

				while (i <= j)
				{
					_idtospan[i++] = nspan;
				}

				//插入
				_freelist[num].Insert(_freelist[num].Begin(), span);
				_freelist[n].Insert(_freelist[n].Begin(), nspan);
				return GetSpanOfPage(num);
			}
		}
		//到这里说明后面没有了，需要申请内存了。
		//申请一个大的。
		Span* span = _spanptr.New();
		span->_freelist = SystemAlloc(PAGE_SIZE - 1);
		_freelist[PAGE_SIZE - 1].Insert(_freelist[PAGE_SIZE - 1].Begin(), span);

		//初始化页号和页数加
		span->_pagecount = PAGE_SIZE - 1;
		span->_pageid = SizeNum::PageId(span->_freelist);

		//这几页加入到map中。

		TYPE begin = span->_pageid, end = span->_pageid + span->_pagecount - 1;

		while (begin <= end)
		{
			_idtospan[begin++] = span;
		}
		//std::cout << span << std::endl;

		return GetSpanOfPage(num);
	}
}


void PageCache::ReleseSpanToPageCache(Span* span)
{
	//直接返还会造成小页非常多，所以我们需要进行合并
	//看前面和后面的页，所以idtospan中需要Span的首尾对应的页和span关系

	//span的前面
	if (span->_pagecount > PAGE_SIZE - 1)
	{
		SystemFree(span->_freelist);
		_idtospan.erase(span->_pageid);
		_spanptr.Delete(span);
	}
	else

	{
		while (1)
		{

			//前面没有
			// 没有还完
			// 正在使用
			//加上前面的大于128页
			if (_idtospan.find(span->_pageid - 1) == _idtospan.end() ||
				_idtospan[span->_pageid - 1]->_count != 0 ||
				_idtospan.find(span->_pageid - 1)->second->_isUse ||
				span->_pagecount + _idtospan.find(span->_pageid - 1)->second->_pagecount >= PAGE_SIZE
				)
			{
				break;
			}
			Span* prev = _idtospan.find(span->_pageid - 1)->second;
			//到这里说明可以合并了。
			prev->_pagecount += span->_pagecount;
			_idtospan.erase(span->_pageid);
			_idtospan.erase(span->_pageid - 1);
			_idtospan[span->_pageid + span->_pagecount - 1] = prev;

			Span* spanPrev = span->_prev;
			Span* spanNext = span->_next;

			spanPrev->_next = spanNext;
			spanNext->_prev = spanPrev;

			_freelist[prev->_pagecount].Insert(_freelist[prev->_pagecount].End(), prev);
			_spanptr.Delete(span);
		}

		//后面
		while (1)
		{
			TYPE pageid = span->_pageid + span->_pagecount;

			//后面没有
			// 没有还完
			// 正在使用
			//加上后面的大于128页
			if (_idtospan.find(pageid) == _idtospan.end() ||
				_idtospan[pageid]->_count != 0 ||
				_idtospan.find(pageid)->second->_isUse ||
				span->_pagecount + _idtospan.find(pageid)->second->_pagecount >= PAGE_SIZE
				)
			{
				break;
			}

			//到这里说明可以合并了。
			Span* next = _idtospan.find(pageid)->second;

			span->_pagecount += next->_pagecount;
			_idtospan.erase(next->_pageid);
			_idtospan.erase(next->_pageid - 1);
			_idtospan[next->_pageid + next->_pagecount - 1] = span;

			Span* spanPrev = next->_prev;
			Span* spanNext = next->_next;

			spanPrev->_next = spanNext;
			spanNext->_prev = spanPrev;

			_freelist[span->_pagecount].Insert(_freelist[span->_pagecount].End(), span);
			_spanptr.Delete(span);
		}
	}
}