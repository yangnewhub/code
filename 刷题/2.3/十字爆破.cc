#include <iostream>
#include <vector>
using namespace std;

int main()
{
    int n,m;
    std::cin>>n>>m;
    vector<vector<long long>> nums(n+1,vector<long long>(m+1));
    for(int i=1;i<=n;i++)
    {
        for(int j=1;j<=m;j++)
        {
            std::cin>>nums[i][j];
            nums[i][0]+=nums[i][j];
            nums[0][j]+=nums[i][j]; 
        }
    }
    
    for(int i=1;i<=n;i++)
    {
        for(int j=1;j<=m;j++)
        {
            std::cout<<nums[i][0]+nums[0][j]-nums[i][j]<<" ";
        }
        std::cout<<std::endl;
    }
    
    return 0;
}