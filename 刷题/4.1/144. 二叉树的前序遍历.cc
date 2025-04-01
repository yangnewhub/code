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
        vector<int> preorderTraversal(TreeNode* root) {
                    vector<int> nums;
            if(root==nullptr) return nums;
    
            stack<TreeNode*> st;
            TreeNode* cur = root;
    
            while(!st.empty()||cur)
            {
                while(cur)
                {
                    st.push(cur);
                    nums.push_back(cur->val);
                    cur=cur->left;
                }
                //最左边了，右边有咩有，空||访问过
                cur = st.top();
                st.pop();
                cur=cur->right;
                
            }
            return nums;
        }
    };