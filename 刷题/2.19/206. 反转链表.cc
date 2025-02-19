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
        ListNode* reverseList(ListNode* head) {
            ListNode *newhead=nullptr;
            ListNode *cur=head;
            while(cur)
            {
                if(newhead==nullptr)
                {
                    newhead=cur;
                    ListNode *tail=cur->next;
                    cur->next=nullptr;
                    cur=tail;
                }
                else 
                {
                    ListNode *tail=cur->next;
                    cur->next=newhead;
                    newhead =cur;
                    cur=tail;
                }
            }
            return newhead;
    
        }
    };