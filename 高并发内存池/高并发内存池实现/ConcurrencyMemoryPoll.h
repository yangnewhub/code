#pragma once

#include <iostream>
#include <vector>
#include "Comm.h"
using std::cout;
using std::endl;

#ifdef _WIN32
#include<windows.h>
#else
// 
#endif



template <class T>
class ObjectPool
{
public:
	T* New()
	{
		T* obj = nullptr;
		//���ж��ڴ湻�����������Ļ��ٿ��ڴ�
		//��Ȼmemory���ǿգ���������ʱ�ڴ治��Ҳ��Ҫȥ����
		//����ʱ���������������л�ȡ

		//��
		if (!_freeList)
		{
			if (_memorysize < sizeof(T))
			{
				int size = 128 * 1024;
				//_memory = (void*)malloc(size);
				_memory = SystemAlloc(size >> 13);

				_memorysize = size;
			}

			//�ڴ湻�ˣ������ڴ�
			//�����������ٿ���һ��ָ��Ĵ�С
			int memorysize = sizeof(T) < sizeof(void*) ? sizeof(void*) : sizeof(T);

			obj = (T*)_memory;
			_memory = (char*)_memory + memorysize;
			_memorysize -= memorysize;
		}
		else
		{
			obj = (T*)_freeList;
			_freeList = *(void**)_freeList;
		}

		new(obj)T;
		return obj;
	}
	void Delete(T* obj)
	{
		obj->~T();
		//ͷ��
		*(void**)obj = _freeList;
		_freeList = obj;
	}
private:
	void* _memory = nullptr;
	void* _freeList = nullptr;
	int _memorysize = 0;
};

//struct TreeNode
//{
//	int _val;
//	TreeNode* _left;
//	TreeNode* _right;
//
//	TreeNode()
//		:_val(0)
//		, _left(nullptr)
//		, _right(nullptr)
//	{}
//};
//void TestObjectPool()
//{
//	// �����ͷŵ��ִ�
//	const size_t Rounds = 5;
//
//	// ÿ�������ͷŶ��ٴ�
//	const size_t N = 100000;
//
//	std::vector<TreeNode*> v1;
//	v1.reserve(N);
//
//	size_t begin1 = clock();
//	for (size_t j = 0; j < Rounds; ++j)
//	{
//		for (int i = 0; i < N; ++i)
//		{
//			v1.push_back(new TreeNode);
//		}
//		for (int i = 0; i < N; ++i)
//		{
//			delete v1[i];
//		}
//		v1.clear();
//	}
//
//	size_t end1 = clock();
//
//	std::vector<TreeNode*> v2;
//	v2.reserve(N);
//
//	ObjectPool<TreeNode> TNPool;
//	size_t begin2 = clock();
//	for (size_t j = 0; j < Rounds; ++j)
//	{
//		for (int i = 0; i < N; ++i)
//		{
//			v2.push_back(TNPool.New());
//		}
//		for (int i = 0; i < N; ++i)
//		{
//			TNPool.Delete(v2[i]);
//		}
//		v2.clear();
//	}
//	size_t end2 = clock();
//
//	cout << "new cost time:" << end1 - begin1 << endl;
//	cout << "object pool cost time:" << end2 - begin2 << endl;
//}