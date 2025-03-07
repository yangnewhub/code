//前缀和 + 线性事件
class Solution {
    public:
        int subarraySum(vector<int>& nums, int k) {
            unordered_map<int,int>hash;
            int n = nums.size();
            int sum=0;
            int ret=0;
            hash[0]=1;
            for(int i=0;i<n;i++)
            {
                sum+=nums[i];
                ret+=hash[sum - k];
                hash[sum]++;
    
            }
            return ret;
        }
    };