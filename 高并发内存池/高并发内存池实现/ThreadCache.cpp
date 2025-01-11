#include "Comm.h"
#include "ThreadCache.h"
#include "CentralCache.h"

void* ThreadCache::New(size_t size)
{
	assert(size > 0);


	//�õ��������Ͷ�Ӧ���±�
	int alignum = SizeNum::AlignNum(size);
	int index = SizeNum::Index(size);



	/*int i = 0;
	for (void* p = _freelist[index].FreeListPtr(); p; p = NextPtr(p))
	{
		i++;
	}
	if (i != _freelist[index].Size())
	{
		assert(false);
	}*/

	if (!_freelist[index].Empty())
	{
		return _freelist[index].Pop();
	}
	else
	{
		//��central cache�л�ȡ�ڴ�
		//sizeԽ���õ�Ӧ��ԽС��ԽС�Ļ�õ�Ӧ��Խ��
		//ÿ������MaxSize++;
		int num = min(_freelist[index].MaxSize(), SizeNum::Num(alignum));
		_freelist[index].MaxSize()++;


		void* begin = nullptr;
		void* end = nullptr;

		int n = CentralCache::GetOneInstance()->GetMemoryOfCentralCache (begin, end, alignum, index, num);

		if (n == 0)
		{
			std::cout << "ThreadCache error New" << std::endl;
			return nullptr;
		}
		
		
		//int i = 0;
		//void* cut = begin;
		//while (cut != end)
		//{
		//	cut = NextPtr(cut);
		//	i++;
		//}
		//
		//if (i + 1 != n)
		//{
		//	assert(false);
		//}

		if (n == 1)
		{
			_freelist[index].Push(begin);
		}
		else
		{
			_freelist[index].PushForRange(begin, end, n);
		}


		return _freelist[index].Pop();

	}
}
void ThreadCache::Delete(void* ptr, int size)
{
	assert(ptr);
	assert(size > 0);
	int index = SizeNum::Index(size);
	_freelist[index].Push(ptr);

	//��ʣ��������������ֵʱ�����з�����


	//i��������һ�����룬����û�����⣬����û������
	//��ɾ��

	if (_freelist[index].Size() >= _freelist[index].MaxSize())
	{
		//int i = 0;
		//void* p = _freelist[index].FreeListPtr();
		//for (; p; p = NextPtr(p))
		//{
		//	i++;
		//}
		//if(i != _freelist[index].Size())
		//{
		//	assert(false);
		//}

		

		void* begin = _freelist[index].FreeListPtr();
		void* end = begin;
		
		_freelist[index].Size()--;
		while (_freelist[index].Size() > 1)
		{
			end = NextPtr(end);
			_freelist[index].Size()--;
		}

		_freelist[index].FreeListPtr() = NextPtr(end);
		NextPtr(end) = nullptr;



		CentralCache::GetOneInstance()->ReleselistToCentralCache(begin, end, _freelist[index].Size(), index);
	}
}
