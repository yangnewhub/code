class Solution {
    public:
        int removeDuplicates(vector<int>& nums) {
            map<int,int> hash;
            for(auto x:nums) hash[x]++;
            int j=0;
            for(int i=0;i<nums.size();i++)
            {
                if(hash[nums[i]]>0)
                {
                    nums[j++]=nums[i];
                    hash[nums[i]]=0;
                }
            }
            return j;
        }
    };