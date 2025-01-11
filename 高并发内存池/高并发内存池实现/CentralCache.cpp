#include "Comm.h"
#include "CentralCache.h"
#include "PageCache.h"

//
CentralCache CentralCache::_centralcache;

Span* CentralCache::GetNoEmpty(SpanList& list, int alignum, int index)
{
	//�յĻ�����pagecache�л�ȡȻ����룬Ȼ���ȡ��
	for (Span* it = list.Begin(); it != list.End(); it = it->_next)
	{
		//freelist��Ϊ���ҹ�����һ��
		if (it->_freelist != nullptr && alignum < SizeNum::PageEndPtr(it->_freelist) - (TYPE)it->_freelist)
		{
			////span�ǵڶ���ʹ��
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
	//���������û���ˣ���ҪGetSpanOfPage();
	//��ҳ�����ڴ�
	//�����ҳ��: ���ڴ�Ķ������й�
	//�ڴ������ԽС�������ҳ����ԽС��
	//�ڴ������Խ�������ҳ����Խ�ࡣ

	//��ȡ��ҳ��
	int n = SizeNum::NumOfPage(alignum);
	//�����Ƚ�Ͱ���⿪
	_spanlist[index]._mux.unlock();

	//��Ϊ���ڴ��и������������Ҫһ�����������
	PageCache::GetOneInstance()->_pmux.lock();

	Span* span = PageCache::GetOneInstance()->GetSpanOfPage(n);
	span->_isUse = true;
	span->_size = alignum;

	PageCache::GetOneInstance()->_pmux.unlock();


	//�����freelist��
	void* begin = span->_freelist;
	void* end = (void*)SizeNum::PageEndPtr(begin);
	while ((char*)begin +alignum < (char*)end)
	{
		NextPtr(begin) = (char*)begin + alignum;
		begin = NextPtr(begin);
	}
	NextPtr(begin) = nullptr;

	//���½�Ͱ������
	_spanlist[index]._mux.lock();

	list.Insert(list.End(), span);
	return list.Begin();

}


//��centralcache�л�ȡ�ڴ�
int CentralCache::GetMemoryOfCentralCache(void*& begin, void*& end, int alignum, int index, int num)
{
	_spanlist[index]._mux.lock();
	//��ȡһ���ǿյ�Span
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
	
	//��ȥcount����
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

		//�õ���Ӧ��span
		Span* span = PageCache::GetOneInstance()->_idtospan[SizeNum::PageId(it)];

		PageCache::GetOneInstance()->_pmux.unlock();

		//��it���뵽span��

		NextPtr(it) = span->_freelist;
		span->_freelist = it;
		span->_count--;

		//��ȫ������ʱ�ͽ�span������pagecache

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