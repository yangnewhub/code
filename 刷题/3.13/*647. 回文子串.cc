//回文串都是列举区间首尾，
//以i开始，j结束的字符串是不是回文串
//需要知道s[i]?s[j]   == | ！= 长度为1，2,>2
class Solution {
    public:
        int countSubstrings(string s) {
            int n =s.size();
            vector<vector<bool>> dp(n,vector<bool>(n,false));
            int ret =0;
            for(int i =n-1;i>=0;i--)
            {
                for(int j =i;j<n;j++)
                {
                    if(s[i]==s[j]) dp[i][j] = i + 1 < j ? dp[i + 1][j - 1] : true;;
                    if(dp[i][j]) ret++;
                }
            }
            return ret;
        }
    };