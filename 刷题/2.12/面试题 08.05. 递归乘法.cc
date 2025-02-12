class Solution {
    public:
        int multiply(int A, int B) {
            if(A<B) swap(A,B);
            int ret=0;
            for(int i=0;i<B;i++)
                ret+=A;
            return ret;
        }
    };