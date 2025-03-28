/*
// Definition for a Node.
class Node {
public:
    int val;
    vector<Node*> children;

    Node() {}

    Node(int _val) {
        val = _val;
    }

    Node(int _val, vector<Node*> _children) {
        val = _val;
        children = _children;
    }
};
*/

class Solution {
    public:
        vector<vector<int>> levelOrder(Node* root) {
            queue<Node*> q;
            vector<vector<int>> ret;
            if(!root) return ret;
    
            q.push(root);
    
            while(q.size())
            {
                //先统计这层有几个
                int sz = q.size();
                vector<int> nums;
                for(int i =0;i<sz;i++)
                {
                    Node * t = q.front();
                    q.pop();
                    nums.push_back(t->val);
                    for(auto x: t->children)
                    {
                        if(x) q.push(x);
                    }
                }
                ret.push_back(nums);
            }
            return ret;
        }
    };