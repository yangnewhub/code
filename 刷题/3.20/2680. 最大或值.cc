class Solution {
    public:
        long long maximumOr(vector<int>& nums, int k) {
            /*
                前缀与和后缀与然后列举
            */
            int n =nums.size();
            if(n==1) return (long long)nums[0]<<k;
            vector<long long> f(n),g(n);
    
            for(int i=1;i<n;i++)  f[i]=f[i-1]|nums[i-1];
            for(int i=n-2;i>=0;i--) g[i]=g[i+1]|nums[i+1];
    
            long long ret=0;
            for(int i=0;i<n;i++)
            {
                long long tmp= f[i]|g[i];
                //特别注意这里nums【i】要先转为long long
                ret =max(ret,tmp|((long long)nums[i]<<k));
            }
            return ret;
        }
    };