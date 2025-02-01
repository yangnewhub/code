#include <iostream>
#include <string>
#include <vector>
#include <queue>
using namespace std;




struct ListNode {
    int val;
    struct ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

class Solution {
public:
    /**
     * 代码中的类名、方法名、参数名已经指定，请勿修改，直接返回方法规定的值即可
     *
     * 
     * @param lists ListNode类vector 
     * @return ListNode类
     */
    struct cmp
    {
        bool operator()(ListNode*l1,ListNode*l2 )
        {
            return l1->val>l2->val;
        }
    };

    ListNode* mergeKLists(vector<ListNode*>& lists) {
        // write code here
        priority_queue<ListNode*,vector<ListNode*>,cmp> heap;

        for(auto l:lists)
        {
            if(l)
                heap.push(l);
        }

        ListNode *head=new ListNode(0);
        ListNode*cur=head;
        while(!heap.empty())
        {
            ListNode* tmp=heap.top();
            cur->next=tmp;
            cur=cur->next;
            heap.pop();
            if(tmp->next)
            {
                heap.push(tmp->next);
            }
        }
        return head->next;
    }
};