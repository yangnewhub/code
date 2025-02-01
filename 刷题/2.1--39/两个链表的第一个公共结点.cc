/*
struct ListNode {
	int val;
	struct ListNode *next;
	ListNode(int x) :
			val(x), next(NULL) {
	}
};*/
class Solution {
private:
	int n=0;
	int m=0;
public:
    ListNode* FindFirstCommonNode( ListNode* pHead1, ListNode* pHead2) {
		//先计算长度
        ListNode* cur=pHead1;
		while(cur)
		{
			n++;
			cur=cur->next;
		}

		cur=pHead2;
		while(cur)
		{
			m++;
			cur=cur->next;
		}

		if(n<m)
		{
			swap(pHead1,pHead2);
			swap(n,m);
		}

		int tmp=n-m;
		while(tmp--)
			pHead1=pHead1->next;

		while(m--)
		{
			if(&pHead1->val!=&pHead2->val)
			{
				pHead1=pHead1->next;
				pHead2=pHead2->next;

			}
			else
			{
				return pHead1;
			}
		}
		return nullptr;
    }
};
