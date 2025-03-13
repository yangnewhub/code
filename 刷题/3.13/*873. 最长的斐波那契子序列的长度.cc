//以当前位置为结尾，单个的话就会不知到前一个
//所以列举出以i 和j为结尾的，保证i<j只需呀找到以 k  i为结尾的就可以了
class Solution {
    public:
        int lenLongestFibSubseq(vector<int>& arr) {
            int n = arr.size();
            vector<vector<int>> dp(n,vector<int>(n,2));
            map<int,int> hash;
            for(int i =0;i<n;i++)hash[arr[i]]=i;
            int ret = 2;
            for(int i =0;i<n;i++)
            {
                for(int j = i+1;j<n;j++)
                {
                    auto it = hash.find(arr[j]-arr[i]);
                    if(it !=hash.end()&&it->second<i)
                    {
                        dp[i][j]=dp[it->second][i]+1;
                        ret = max(ret,dp[i][j]);
                    } 
                }
            }
            return ret==2?0:ret;
        }
    };s