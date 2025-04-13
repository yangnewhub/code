class Solution {
    public:
        int hIndex(vector<int>& citations) {
            
            int n = citations.size();
            sort(citations.begin(),citations.end());
            
            int num = citations[citations.size()-1];
            while(num)
            {
                int count =0;
                for(auto x:citations)
                {
                    if(x>=num) count++;
                }
                if(count >= num) return num;
                num--;
            }
    
            return 0;
        }
    };