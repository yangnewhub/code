#include <iostream>
#include <vector>
#include <map>
using namespace std;

int main()
{
    int n,m;
    std::cin>>n>>m;
    vector<int> nums(n);
    int maxnum=0;
    for(int i=0;i<n;i++)
    {
        std::cin>>nums[i];
    }
    map<int,int> hash;
    for(auto x:nums) 
    {
        hash[x]++;
        maxnum=max(maxnum,hash[x]);
    }
    if(hash.size()>m) 
    {
        std::cout<<-1<<std::endl;
        return 0;
    }
    int left =0,right=maxnum;
    while(left<right)
    {
        int sum=0;
        int mid=(right-left)/2+left;
        for(auto x:hash)
        {
            sum+=x.second/mid+(x.second%mid==0?0:1);
        }
        if(sum>m) left=mid+1;
        else 
        {
            right=mid;
        }
    }    
    std::cout<<left<<std::endl;
    return 0;
}