//34
#include <iostream>
#include <vector>
#include <map>
using namespace std;

int gcd(long long a,long long b)
{
    if(b==0) return a;
    return gcd(b,a%b);
}

int main()
{
    int n;
    cin>>n;
    int count=0;
    vector<int> nums(n);
    map<int,int>hash;
    for(int i =1;i<=n;i++)
    {
        if(gcd(999999999,i)!=1) 
        {
            count++;
            hash[i]++;
        }
    }
    if(count+1<n-count) 
    {
        cout<<"Baka!";
        return 0;
    }
    auto it = hash.begin();
    for(int i = 1;i<n;i+=2)
    {
        nums[i]=it->first;
        it++;
    }
    int j = 0;
    while(it!=hash.end())
    {
        nums[j]=it->first;
        it++;
        j+=2;
    }
    for(int i=1;i<=n;i++)
    {
        if(hash[i]==0)
        {
            nums[j]=i;
            j+=2;
        }
    }
    for(auto x: nums) cout<<x<<" ";
}