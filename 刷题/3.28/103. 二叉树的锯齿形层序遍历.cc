/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
    public:
        vector<vector<int>> zigzagLevelOrder(TreeNode* root) {
            queue<TreeNode*> q;
            vector<vector<int>> ret;
            if(!root) return ret;
    
            q.push(root);
            int count = 0;
            while(q.size())
            {
                int sz = q.size();
                vector<int> nums;
        
                for(int i =0;i<sz;i++)
                {
                    TreeNode * t = q.front();
                    q.pop();
                    nums.push_back(t->val);
                
                  
                    if(t->left) q.push(t->left);
                    if(t->right) q.push(t->right);
                
                } 
                if(count%2==1)   reverse(nums.begin(),nums.end());
              
    
                ret.push_back(nums);
                count++;
              
            }
            return ret;
        }
    };