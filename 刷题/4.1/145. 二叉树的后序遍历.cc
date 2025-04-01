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
        vector<int> postorderTraversal(TreeNode* root) {
            vector<int> nums;
            if(root==nullptr) return nums;
            stack<TreeNode*> st;
            st.push(root);
            TreeNode * cur=root->left;
            TreeNode * prev=nullptr;
            while(!st.empty())
            {
                while(cur)
                {
                    st.push(cur);
                    cur=cur->left;
                }
    
                //到这里左子树就完了，
                auto t = st.top();
                if(t->right==nullptr||prev==t->right)
                {
                    nums.push_back(t->val);
                    st.pop();
                    prev= t;
                }
                else 
                {
                    cur=t->right;
                }
            }
            return nums;
        }
    };