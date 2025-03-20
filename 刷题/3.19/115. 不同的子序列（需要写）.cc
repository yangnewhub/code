class Solution {
    public:
        int numDistinct(string s, string t) {
            /*
                0,i   0,j 的子序列的个数
                根据s[j]选不选先分一次类
                然后再根据想不想等分一次类
            */
            int m=s.size(),n=t.size();
            vector<vector<double>>dp(n+1,vector<double>(m+1));
            for(int i=0;i<=m;i++) dp[0][i]=1;
            s=' '+s;
            t=' '+t;
            for(int i=1;i<=n;i++)
                for(int j=1;j<=m;j++)
                {
                    dp[i][j]+=dp[i][j-1];
                    if(s[j]==t[i])
                    dp[i][j]+=dp[i-1][j-1];
                }
            return dp[n][m];
        }
    };