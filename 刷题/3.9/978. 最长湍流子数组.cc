class Solution {
    public:
        int maxTurbulenceSize(vector<int>& arr) {
            int n =arr.size();
            vector<int> f(n+1,1),g(n+1,1);
            int ret=1;
            for(int i=2;i<=n;i++)
            {
                if(arr[i-1]>arr[i-2]) f[i]=g[i-1]+1;
                else if(arr[i-1]<arr[i-2]) g[i]=f[i-1]+1;
                ret=max(ret,max(f[i],g[i]));
            }
            return ret;
        }
    };