class Solution {
public:
    int maxSubarraySumCircular(vector<int>& nums) {
        int n =nums.size();
        int gmax = INT_MIN,gmin=INT_MAX;
        vector<int> f(n+1),g(n+1);
        int sum=0;
        //分两种情况，头尾和中间，中间就是找最大，左右的就是找最小
        for(int i=1;i<=n;i++)
        {
            sum+=nums[i-1];
            f[i]=max(f[i-1]+nums[i-1],nums[i-1]);
            gmax = max(f[i],gmax);

            g[i]=min(g[i-1]+nums[i-1],nums[i-1]);
            gmin = min(g[i],gmin);
        }
        return sum == gmin? gmax:max(gmax,sum-gmin);
    }
};