//子序列问题，先拆分为最长递增子序列，再加一个个数，统计他的个数
//个数分为 最长子序列长度是否改变 


class Solution {
    public:
        int findNumberOfLIS(vector<int>& nums) {
            int n = nums.size();
            vector<int> dp(n,1),count(n,1);
            int len =1;
            for(int i=1;i<n;i++)
            {
                for(int j =0;j<i;j++)
                {
                    if(nums[i]>nums[j])
                    {
                        if(dp[j]+1==dp[i]) count[i]+=count[j];
                        else if(dp[j]+1>dp[i])
                        {
                            dp[i]=dp[j]+1;
                            len = max(len,dp[i]);
                            count[i]=count[j];
                        }
                    }
                }
            }
    
            int ret=0;
            for(int i =0;i<n;i++)
            {
                if(len == dp[i]) ret+=count[i];
            }
            
            return ret;
    
        }
    };