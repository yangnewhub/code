#include <iostream>

#include "ThreadCache.h"
#include "RequestAndRelease.h"

#include <time.h>
#include <vector>
#include<thread>
#include <cstdio>
#include "Comm.h"
using std::cout;
using std::endl;
struct TreeNode
{
	int _val;
	TreeNode* _left;
	TreeNode* _right;

	TreeNode()
		:_val(0)
		, _left(nullptr)
		, _right(nullptr)
	{}
};
void TestObjectPool()
{
	// 申请释放的轮次
	const size_t Rounds = 5;

	// 每轮申请释放多少次
	const size_t N = 100000;

	std::vector<TreeNode*> v1;
	v1.reserve(N);

	size_t begin1 = clock();
	for (size_t j = 0; j < Rounds; ++j)
	{
		for (int i = 0; i < N; ++i)
		{
			v1.push_back(new TreeNode);
		}
		v1.clear();
	}

	size_t end1 = clock();

	std::vector<TreeNode*> v2;
	v2.reserve(N);

	size_t begin2 = clock();
	for (size_t j = 0; j < Rounds; ++j)
	{
		for (int i = 0; i < N; ++i)
		{
			v2.push_back((TreeNode*)AllocMemory(sizeof(TreeNode)));
		}
		v2.clear();
	}
	size_t end2 = clock();

	cout << "new cost time:" << end1 - begin1 << endl;
	cout << "object pool cost time:" << end2 - begin2 << endl;
}
void New1()
{
	for (int i = 0; i < 10000; i++)
	{
		void* p = AllocMemory(1);
	}
}
void New2()
{
	for (int i = 0; i < 10000; i++)
	{
		void *p = AllocMemory(1);
	}
}


void TestObjPool3()
{
	int sum = 0;
	std::vector<char*> v1;
	for (size_t i = 1; i < 10000; i++)
	{
		v1.push_back((char*)AllocMemory(i));
		//v1.push_back((char*)AllocMemory(1));

		sum += SizeNum::AlignNum(i);

		printf("i: %d p1: %x,sum: %d\n", i, v1[i - 1], sum);
	}
	for (size_t i = 1; i < 10000; i++)
	{
		std::cout << i << std::endl;
		DeleteMemory(v1[i - 1]);
		//DeleteMemory(v1[i - 1], 1);

	}
}
void TestObjPool4()
{
	int sum = 0;
	std::vector<char*> v1;
	for (size_t i = 1; i < 1000; i++)
	{
		v1.push_back((char*)AllocMemory(i));
		//v1.push_back((char*)AllocMemory(6));

		sum += SizeNum::AlignNum(i);

		printf("i: %d p1: %x,sum: %d\n", i, v1[i - 1], sum);
	}
	for (size_t i = 1; i < 1000; i++)
	{
		std::cout << i << std::endl;
		DeleteMemory(v1[i - 1]);
	}
}



void TestObjectPool5()
{
	char* p2 = (char*)AllocMemory(1);
	printf("p2: %x\n", p2);
	char* p3 = (char*)AllocMemory(1);
	printf("p3: %x\n", p3);
	char* p4 = (char*)AllocMemory(1);
	printf("p4: %x\n", p4);

	DeleteMemory(p2);
	DeleteMemory(p3);
	DeleteMemory(p4);
}
void TestObjPool6()
{
	for(int i =0 ;i < 1000;i++)
	{
		AllocMemory(257 * 1024);

	}

}

void TestObjPool7()
{
	int sum = 0;
	std::vector<char*> v1;
	for (size_t i = 1; i < 1000; i++)
	{
		v1.push_back((char*)AllocMemory(257 * 1024 + i));

		sum += SizeNum::AlignNum(257 * 1024 + i);

		printf("i: %d p1: %x,sum: %d\n", i, v1[i - 1], sum);
	}
	for (size_t i = 1; i < 1000; i++)
	{
		std::cout << i << std::endl;
		DeleteMemory(v1[i - 1]);
	}
}

//int main()
//{
//	//TestObjectPool();
//
//	//std::thread thread1(TestObjPool3);
//	//std::thread thread2(TestObjPool4);
//	//thread1.join();
//	//thread2.join();
//
//
//	//TestObjPool3();
//	//TestObjPool4();
//
//	//TestObjectPool5();
//	//TestObjPool6();
//	//TestObjPool7();
//
//	return 0;
//}