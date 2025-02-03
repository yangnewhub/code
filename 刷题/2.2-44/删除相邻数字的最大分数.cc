#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
using namespace std;

int main() 
{
    int n;
    std::cin>>n;
    vector<int> nums(n);
    for(int i=0;i<n;i++)
    {
        std::cin>>nums[i];
    }
    nums.push_back(-1);
    //先进行排序
    //有n+1个
    sort(nums.begin(),nums.end());

    vector<int> f(n+1),g(n+1);
    int ret=0;
    for(int i=1;i<=n;i++)
    {
        if(nums[i-1]+1==nums[i]) f[i]=g[i-1]+nums[i];
        else f[i]=max(f[i-1],g[i-1])+nums[i];

        g[i]=max(f[i-1],g[i-1]);
        ret=max(ret,max(g[i],f[i]));
    }
    std::cout<<ret<<std::endl;
    return 0;
}
// 64 位输出请用 printf("%lld")