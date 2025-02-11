class MinStack {
    private:
        
        vector<int> s,min_s;
    public:
        MinStack() {
            
        }
        
        void push(int val) {
            s.push_back(val);
            if(min_s.empty()||min_s.back()>=val) min_s.push_back(val);
        }
        
        void pop() {
            int val=s[s.size()-1];
            if(min_s.back()==s.back()) min_s.pop_back();
    
            s.pop_back();
    
        }
        
        int top() {
            return s.back();
        }
        
        int getMin() {
            return min_s.back();
        }
    };
    
    /**
     * Your MinStack object will be instantiated and called as such:
     * MinStack* obj = new MinStack();
     * obj->push(val);
     * obj->pop();
     * int param_3 = obj->top();
     * int param_4 = obj->getMin();
     */