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
        void reorderList(ListNode* head) {
           vector<int> nums;
           ListNode *cur=head;
           while(cur) 
           {
                nums.push_back(cur->val);
                cur=cur->next;
           }
           int left=0,right=nums.size()-1;
           cur=head;
           while(left<=right)
           {
                if(cur)
                {
                    cur->val=nums[left++];
                    cur=cur->next; 
                }
                if(cur)
                {
                    cur->val=nums[right--];
                    cur=cur->next;
                }
           }
        }
    };