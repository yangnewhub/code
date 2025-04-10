class Solution {
    public:
        bool canJump(vector<int>& nums) {
            int n =nums.size();
            map<int,int> hash;
            for(int i=0;i<n;i++)
            {
                if(nums[i]==0) hash[i]++;
            } 
            if(hash.size()==0) return true;
            if(n==1) return true;
            for(auto it:hash)
            {
                if(check(nums,it.first)==false) return false;
            }
            return true;
        }
        bool check(vector<int>& nums,int index)
        {
            for(int i =index-1;i>=0;i--)
            {
                if(nums[i]+i>index||nums[i]+i==nums.size()-1) return true;
            }
            return false;
        }
    
    };