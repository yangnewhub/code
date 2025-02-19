class Solution {
    public:
        long long maxWeight(vector<int>& pizzas) {
            sort(pizzas.begin(),pizzas.end());
            int n=pizzas.size();
            int m=n/4;
            int cur=n-1,prev=cur-m/2-1-m%2;
            long long ret=0;
            for(int i=0;i<m;i++)
            {
                if(i%2==0) ret+=pizzas[cur--];
                else 
                {
                    ret+=pizzas[prev];
                    prev-=2;
                }
                
            }
            return ret;
        }
    };