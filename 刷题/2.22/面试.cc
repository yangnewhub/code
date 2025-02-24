#include <iostream>
#include <vector>
#include <stack>
using namespace std;

class ListNode
{
public:
	ListNode(int val)
		:_val(val), _left(nullptr),_right(nullptr)
	{}
public:
	int _val;
	ListNode* _left;
	ListNode* _right;

};

void dfs(ListNode* root)
{
	if (root == nullptr) return;
	std::cout << root->_val <<std::endl;
	dfs(root->_left);
	dfs(root->_right);
}

void mydfs(ListNode* root)
{
	stack<ListNode*> s;
	s.push(root);
	while(!s.empty())
	{
		ListNode* p=s.top();
		s.pop();
		std::cout<<p->_val<<std::endl;
		if(p->_right) s.push(p->_right);
		if(p->_left) s.push(p->_left);
	}
}
int main()
{
	ListNode* head = new ListNode(0);
	head->_left = new ListNode(8);
	head->_right = new ListNode(7);
	ListNode* cur = head->_left;
	cur->_left = new ListNode(5);
	cur->_right = new ListNode(4);
	cur = head->_right;
	cur->_left = new ListNode(3);
	cur->_right = new ListNode(2);
	dfs(head);


	std::cout<<"-------------------------------"<<std::endl;
	mydfs(head);
}