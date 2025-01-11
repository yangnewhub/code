#include "Comm.h"
#include "CentralCache.h"
#include "PageCache.h"

//
CentralCache CentralCache::_centralcache;

Span* CentralCache::GetNoEmpty(SpanList& list, int alignum, int index)
{
	//空的话就向pagecache中获取然后插入，然后获取。
	for (Span* it = list.Begin(); it != list.End(); it = it->_next)
	{
		//freelist不为空且够开辟一次
		if (it->_freelist != nullptr && alignum < SizeNum::PageEndPtr(it->_freelist) - (TYPE)it->_freelist)
		{
			////span是第二次使用
			//if (!it->_isnew)
			//{
			//	void* begin = it->_freelist;
			//	void* end = (char*)it->_freelist + (it->_pagecount << PAGE_SHIFT);
			//	while (begin < end)
			//	{
			//		NextPtr(begin) = (char*)begin + alignum;
			//		begin = NextPtr(begin);
			//	}
			//	NextPtr(begin) = nullptr;
			//}
			return it;
		}
	}
	//到这里就是没有了，需要GetSpanOfPage();
	//以页申请内存
	//申请的页数: 和内存的对齐数有关
	//内存对齐数越小，申请的页数就越小，
	//内存对齐数越大，申请的页数就越多。

	//获取的页数
	int n = SizeNum::NumOfPage(alignum);
	//可以先将桶锁解开
	_spanlist[index]._mux.unlock();

	//因为有内存切割操作，所以需要一个整体的锁。
	PageCache::GetOneInstance()->_pmux.lock();

	Span* span = PageCache::GetOneInstance()->GetSpanOfPage(n);
	span->_isUse = true;
	span->_size = alignum;

	PageCache::GetOneInstance()->_pmux.unlock();


	//管理好freelist。
	void* begin = span->_freelist;
	void* end = (void*)SizeNum::PageEndPtr(begin);
	while ((char*)begin +alignum < (char*)end)
	{
		NextPtr(begin) = (char*)begin + alignum;
		begin = NextPtr(begin);
	}
	NextPtr(begin) = nullptr;

	//重新将桶锁加上
	_spanlist[index]._mux.lock();

	list.Insert(list.End(), span);
	return list.Begin();

}


//向centralcache中获取内存
int CentralCache::GetMemoryOfCentralCache(void*& begin, void*& end, int alignum, int index, int num)
{
	_spanlist[index]._mux.lock();
	//获取一个非空的Span
	Span* span = GetNoEmpty(_spanlist[index], alignum, index);

	

	begin = span->_freelist;	
	end = begin;


	int count = 1;
	
	while (num > 1&&NextPtr(end))
	{

		end = NextPtr(end);
		count++;
		num--;
	}

	span->_freelist = NextPtr(end);
	
	//出去count个。
	span->_count += count;

	_spanlist[index]._mux.unlock();

	
	//int i = 1;
	//for (void* p = begin; p!=end; p = NextPtr(p))
	//{
	//	i++;
	//}
	//if (i != count)
	//{
	//	assert(false);
	//}



	return count; 
}

void CentralCache::ReleselistToCentralCache(void* begin, void* end, int n, int index)
{
	_spanlist[index]._mux.lock();
	for (int i = 0; i < n; i++)
	{
		void* it = begin;
		begin = NextPtr(begin);

		PageCache::GetOneInstance()->_pmux.lock();

		//得到对应的span
		Span* span = PageCache::GetOneInstance()->_idtospan[SizeNum::PageId(it)];

		PageCache::GetOneInstance()->_pmux.unlock();

		//将it插入到span中

		NextPtr(it) = span->_freelist;
		span->_freelist = it;
		span->_count--;

		//当全部回来时就将span返还到pagecache

		if(span->_count == 0)
		{
			span->_isUse = false;
			//span->_isnew = false;
			PageCache::GetOneInstance()->_pmux.lock();

			PageCache::GetOneInstance()->ReleseSpanToPageCache(span);

			PageCache::GetOneInstance()->_pmux.unlock();

		}

	}
	_spanlist[index]._mux.unlock();

}