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
		//��Ϊ��ʱ��ֱ�ӽ�ҳ����5
		if (_freelist[num].Begin() != _freelist[num].End())
		{
			return _freelist[num].Erase();
		}
		// ��ʱΪ��
		//���¿��������У������иһ����ҳ�и�����ݣ�Ȼ�����
		//����û�о�ȥ�����ڴ棬��Ҫ��������ڴ�ֳ�һ��С��freelist
		for (int i = num; i < PAGE_SIZE; i++)
		{
			if (!_freelist[i].Empty())
			{
				//�и�
				Span* span = _freelist[i].Erase();
				//span��βҳ����
				_idtospan[span->_pageid + num - 1] = span;

				int n = i - num;
				//����nspan
				Span* nspan = _spanptr.New();

				nspan->_freelist = (Span*)((char*)span->_freelist + (num << PAGE_SHIFT));

				span->_pagecount = num;
				nspan->_pagecount = n;
				nspan->_pageid = span->_pageid + num;

				//nspan����βҳ����
				TYPE i = nspan->_pageid, j = nspan->_pageid + nspan->_pagecount - 1;

				while (i <= j)
				{
					_idtospan[i++] = nspan;
				}

				//����
				_freelist[num].Insert(_freelist[num].Begin(), span);
				_freelist[n].Insert(_freelist[n].Begin(), nspan);
				return GetSpanOfPage(num);
			}
		}
		//������˵������û���ˣ���Ҫ�����ڴ��ˡ�
		//����һ����ġ�
		Span* span = _spanptr.New();
		span->_freelist = SystemAlloc(PAGE_SIZE - 1);
		_freelist[PAGE_SIZE - 1].Insert(_freelist[PAGE_SIZE - 1].Begin(), span);

		//��ʼ��ҳ�ź�ҳ����
		span->_pagecount = PAGE_SIZE - 1;
		span->_pageid = SizeNum::PageId(span->_freelist);

		//�⼸ҳ���뵽map�С�

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
	//ֱ�ӷ��������Сҳ�ǳ��࣬����������Ҫ���кϲ�
	//��ǰ��ͺ����ҳ������idtospan����ҪSpan����β��Ӧ��ҳ��span��ϵ

	//span��ǰ��
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

			//ǰ��û��
			// û�л���
			// ����ʹ��
			//����ǰ��Ĵ���128ҳ
			if (_idtospan.find(span->_pageid - 1) == _idtospan.end() ||
				_idtospan[span->_pageid - 1]->_count != 0 ||
				_idtospan.find(span->_pageid - 1)->second->_isUse ||
				span->_pagecount + _idtospan.find(span->_pageid - 1)->second->_pagecount >= PAGE_SIZE
				)
			{
				break;
			}
			Span* prev = _idtospan.find(span->_pageid - 1)->second;
			//������˵�����Ժϲ��ˡ�
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

		//����
		while (1)
		{
			TYPE pageid = span->_pageid + span->_pagecount;

			//����û��
			// û�л���
			// ����ʹ��
			//���Ϻ���Ĵ���128ҳ
			if (_idtospan.find(pageid) == _idtospan.end() ||
				_idtospan[pageid]->_count != 0 ||
				_idtospan.find(pageid)->second->_isUse ||
				span->_pagecount + _idtospan.find(pageid)->second->_pagecount >= PAGE_SIZE
				)
			{
				break;
			}

			//������˵�����Ժϲ��ˡ�
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