class MyCircularQueue {
    private:
        int prev;
        int end;
        vector<int> queue;
    public:
        MyCircularQueue(int k) 
            :prev(0),end(0),queue(k+1)
        {
        }
        
        bool enQueue(int value) {
            if(isFull()) return false;
            queue[end]=value;
            end=(end+1)%queue.size();
            return true;
        }
        
        bool deQueue() {
            if(isEmpty()) return false;
            prev=(prev+1)%queue.size();
            return true;
        }
        
        int Front() {
            if(isEmpty()) return -1;
            return queue[prev];
        }
        
        int Rear() {
            if(isEmpty()) return -1;
            return queue[(queue.size()+end-1)%queue.size()];
        }
        
        bool isEmpty() {
            return prev==end;
        }
        
        bool isFull() {
            return (end+1)%(queue.size())==prev;
        }
    };
    
    /**
     * Your MyCircularQueue object will be instantiated and called as such:
     * MyCircularQueue* obj = new MyCircularQueue(k);
     * bool param_1 = obj->enQueue(value);
     * bool param_2 = obj->deQueue();
     * int param_3 = obj->Front();
     * int param_4 = obj->Rear();
     * bool param_5 = obj->isEmpty();
     * bool param_6 = obj->isFull();
     */