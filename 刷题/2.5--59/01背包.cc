#include <iostream>
#include <vector>
using namespace std;

int main() 
{
    int t;
    std::cin>>t;
    while(t--)
    {
        long long n,m;
        std::cin>>n>>m;
        vector<long long> w(n),v(n);
        for( long long i=0;i<n;i++)std::cin>>w[i]>>v[i];        

        vector< long long> dp(m+1);
        for(long long i=1;i<=n;i++)
        {
            for( long long j=m;j>0;j-- )
            {
        
                if(j>=w[i-1]) dp[j]=max(dp[j],dp[j-w[i-1]]+v[i-1]);
            }
        }
        std::cout<<dp[m]<<std::endl;
    }
    return 0;
}