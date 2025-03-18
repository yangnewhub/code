class Solution {
    public:
        int minInsertions(string s) {
            //i，j最少的插入次数
            //两边相同 i==j||i+1==j  0   dp[i+1][j-1]
            //不同  min(dp[i+1][j],dp[i][j-1])+1;
    
            int n = s.size();
            vector<vector<int>>dp(n,vector<int>(n,0));
    
            for(int i=n-1;i>=0;i--)
            {
                for(int j =i;j<n;j++)
                {
                    if(s[i]==s[j]) 
                        dp[i][j]=i+1>=j?0:dp[i+1][j-1];
                    else dp[i][j]=min(dp[i+1][j],dp[i][j-1])+1;
                }
            }
            return dp[0][n-1];
        }
    };