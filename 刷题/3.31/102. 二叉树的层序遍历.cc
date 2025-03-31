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
        vector<vector<int>> levelOrder(TreeNode* root) {
            vector<vector<int>> ret;   
            if(root==nullptr) return ret;
            queue<TreeNode*> q;
            q.push(root);
            while(q.size())
            {
                int sz = q.size();
                vector<int> nums;
                for(int i = 0; i< sz ;i++)
                {
                    auto t = q.front();
                    q.pop();
                    nums.push_back(t->val);
                    if(t->left) q.push(t->left);
                    if(t->right) q.push(t->right);
                }
                ret.push_back(nums);
            }
     
            return ret;
        }
    };