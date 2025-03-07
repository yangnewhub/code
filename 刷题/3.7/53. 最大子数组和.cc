//线性dp

class Solution {
    public:
        int maxSubArray(vector<int>& nums) {
            int n = nums.size();
            vector<int> dp(n+1);
            int ret=INT_MIN;
            for(int i=1;i<=n;i++)
            {
                dp[i]=max(dp[i-1]+nums[i-1],nums[i-1]);
                ret = max(ret,dp[i]);
            }
            return ret;
        }
    };