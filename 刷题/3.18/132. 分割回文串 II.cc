class Solution {
    public:
        int minCut(string s) {
            //判断i，j是不是回文串
            //【0，i】最少的分割次数
            //
            int n = s.size();
            vector<vector<bool>> dp(n,vector<bool>(n,false));
            for(int i =n-1;i>=0;i--)
            {
                for(int j = i;j<n;j++)
                {
                    if(s[i]==s[j])
                        dp[i][j]=i+1<j?dp[i+1][j-1]:true;
                }
            }
    
            vector<int> f(n,INT_MAX);
            for(int i =0;i<n;i++)
            {
                if(dp[0][i]) f[i]=0;
                else 
                {
                    for(int j =1;j<=i;j++)
                        if(dp[j][i])
                            f[i]=min(f[i],f[j-1]+1); 
                }
            }
            return f[n-1];
        }
    };  