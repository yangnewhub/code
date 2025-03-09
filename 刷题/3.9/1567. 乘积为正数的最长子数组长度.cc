class Solution {
    public:
        int getMaxLen(vector<int>& nums) {
            //判断nums[i]是证书还是负数，然后不同表达式
            int n =nums.size();
            vector<int> f(n+1),g(n+1);
            int ret=0;
            for(int i=1;i<=n;i++)
            {
                if(nums[i-1]>0)
                {
                    f[i]=f[i-1]+1;
                    g[i]=g[i-1]==0?0:g[i-1]+1;
                }
                else if(nums[i-1]<0)
                {
                    f[i]=g[i-1]==0?0:g[i-1]+1;
                    g[i]=f[i-1]+1;
                }
                ret =max (ret,f[i]);
            }
            return ret;
        }
    };