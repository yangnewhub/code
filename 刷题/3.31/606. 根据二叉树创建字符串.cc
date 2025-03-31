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
    private:
        string ret;
    public:
        string tree2str(TreeNode* root) {
            bfs(root);
            return ret;
        }
        void bfs(TreeNode * root)
        {
            if(root==nullptr) return;
            ret+=to_string(root->val);
            if(root->left==nullptr&&root->right==nullptr) return ;
            if(root->left) 
            {
                ret+="(";
                bfs(root->left);
                ret+=")";
            }
            else ret+="()";
            if(root->right)
            {
                ret+="(";
                bfs(root->right);
                ret+=")";
            }
    
        }
    };