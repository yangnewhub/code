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
        int widthOfBinaryTree(TreeNode* root) {
            queue<pair<TreeNode*,unsigned int >> q;
            unsigned int ret=0;
    
            if(!root) return 0;
            q.push({root,1});
            while(q.size())
            {
                int sz = q.size();
                unsigned int left=0,right=0;
                left = q.front().second;
                right = q.back().second;
                ret = max(ret,right-left+1);
    
                for(int i =0;i<sz;i++)
                {
                    auto t = q.front();
                    // if(i==0) left=t.second;
                    // if(i==sz-1) right=t.second;
                    q.pop();
                    TreeNode * p=t.first;
                    if(p->left)q.push({p->left,2*t.second});
                    if(p->right)q.push({p->right,2*t.second+1});
    
                }
               
            }
            return ret;
        }
    };