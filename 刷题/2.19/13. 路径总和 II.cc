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
        vector<vector<int>> ret;
        vector<int> path;
        int sum=0;
    public:
        vector<vector<int>> pathSum(TreeNode* root, int targetSum) {
            dfs(root,targetSum);
            return ret;
        }
        void dfs(TreeNode* root, int targetSum)
        {
            if(root==nullptr) return ;
            path.push_back(root->val);
            sum+=root->val;
            
            //出口
            if(root->left==nullptr&&root->right==nullptr)
            {
                if(sum==targetSum)
                    ret.push_back(path);
                sum-=root->val;
                path.pop_back();
    
                return ;
            }
    
            
            if(root->left) dfs(root->left,targetSum);
     
            if(root->right) dfs(root->right,targetSum);
            sum-=root->val;
            path.pop_back();
    
        }
    };