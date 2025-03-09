//以i位置为结尾，选的最大子序列的长度。需要枚举j位置
class Solution {
    public:
        int lengthOfLIS(vector<int>& nums) {
            int n =nums.size();
            vector<int> dp(n+1,1);
            int ret=1;
            for(int i=1;i<=n;i++)
            {
                for(int j=i;j>=1;j--)
                {
                    if(nums[j-1]<nums[i-1]) dp[i]=max(dp[i],dp[j]+1);
                    ret =max(ret,dp[i]);
                }
            }
            return ret;
        }
    };