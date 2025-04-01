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
        vector<int> inorderTraversal(TreeNode* root) {
            vector<int> nums;
            if(root==nullptr) return nums;
            stack<TreeNode*> st;
            TreeNode* cur = root;
            while(!st.empty()||cur)
            {
                while(cur)
                {
                    st.push(cur);
                    cur=cur->left;
                }
    
                cur = st.top();
                nums.push_back(cur->val);
                st.pop();
    
                cur=cur->right;
                
            }
            return nums;
        }
    };