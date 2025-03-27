class Solution {
    public:
        bool canPartition(vector<int>& nums) {
            int sum=0;
            for(auto x: nums) sum+=x;
            int num=sum/2;
            if(sum%2==1) return false;
    
            int n =nums.size();
            vector<vector<int>> dp(n+1,vector<int>(num+1,-1));
            //dp[i][j]= buxuan dp[i-1][j]
            //xuan  dp[i-1][j-num[i]]+nums[i]
            for(int i = 1;i<=n;i++)
            {
                for(int j = 1;j<=num;j++)
                {
                    dp[i][j] = dp[i-1][j];
                    if(j>=nums[i-1]&&dp[i-1][j-nums[i-1]]!=-1) dp[i][j]=max(dp[i-1][j-nums[i-1]]+nums[i-1],dp[i][j]);
                }
    
            }
            return dp[n][num]!=-1;
        }
    };