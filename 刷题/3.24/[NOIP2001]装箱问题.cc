#include <iostream>
#include <vector>
using namespace std;

int main()
{
    int v,n;
    cin>>v>>n;
    vector<int> nums(n);
    for(int i=0;i<n;i++) cin>>nums[i];
    
    vector<vector<int>>dp(n+1,vector<int>(v+1));
    
    for(int i=1;i<=n;i++)
    {
        for(int j = 1;j<=v;j++)
        {
            dp[i][j]=dp[i-1][j];
            if(j>=nums[i-1]) dp[i][j]=max(dp[i][j],dp[i-1][j-nums[i-1]]+nums[i-1]);
        }
    }
    
    cout<<v-dp[n][v];
    return 0;
}