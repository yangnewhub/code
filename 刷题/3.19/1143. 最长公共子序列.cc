class Solution {
    public:
        int longestCommonSubsequence(string text1, string text2) {
            //dp[i][j]表示从0，i 0,j最长公共子序列的长度
            //判断末尾 相同和不同   相同 dp[i-1][j-1]+1;
            //不同  max(dp[i-1][j],max(dp[i-1][j-1],dp[i][j-1]));
    
            int  n = text1.size(),m=text2.size();
            text1=' '+text1;
            text2=' '+text2;
            vector<vector<int>>dp(n+1,vector<int>(m+1));
            for(int i =1;i<=n;i++)
            {
                for(int j =1;j<=m;j++)
                {
                    if(text1[i]==text2[j]) dp[i][j]= dp[i-1][j-1]+1;
                    else  dp[i][j]=max(dp[i-1][j],max(dp[i-1][j-1],dp[i][j-1]));
                }
            }
            return dp[n][m];
    
        }
    };