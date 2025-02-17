/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
    public:
        ListNode* rotateRight(ListNode* head, int k) {
            int n =0;
            ListNode *cur=head;
            while(cur)
            {
                n++;
                cur=cur->next;
            }
            if(n*k==0) return head;
            k=k%n;
            if(n*k==0) return head;
    
            ListNode *prev;
            prev=cur=head;
            while(k--)
            {
                prev=cur;
                cur=cur->next;
            }
            ListNode *newhead=cur;
            while(cur->next!=nullptr)
            {
                cur=cur->next;
            }
            cur->next=head;
            prev->next=nullptr;
            head=newhead;
            return head;
        }
    };