class Solution {
    public:
        int firstMissingPositive(vector<int>& nums) {
            map<int,int> hash;
            for(auto x:nums) hash[x]++;
            int n=nums.size();
            int ret=1;
            while(true)
            {
                if(hash[ret]==0) return ret;
                ret++;
            }
        }
    };