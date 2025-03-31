/*
// Definition for a Node.
class Node {
public:
    int val;
    Node* left;
    Node* right;

    Node() {}

    Node(int _val) {
        val = _val;
        left = NULL;
        right = NULL;
    }

    Node(int _val, Node* _left, Node* _right) {
        val = _val;
        left = _left;
        right = _right;
    }
};
*/
class Solution {
    private:
        Node* prev=nullptr;
        Node * head=nullptr;
    public:
        Node* treeToDoublyList(Node* root) {
            if(!root) return root;
            InOrder(root);
            head->left=prev;
            prev->right=head;
            return head;
        }
    
        void InOrder(Node * root)
        {
            if(!root) return ;
    
            InOrder(root->left);
    
            root->left=prev;
            if(prev) prev->right=root;
            if(prev==nullptr) head=root;
    
            prev=root;  
    
            InOrder(root->right);
    
        }
    };