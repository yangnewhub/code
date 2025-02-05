//排序

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

const int N= 2e5+10;
typedef pair<long long,long long> PII;

PII arr[N];
long long n,k;

int main()
{  
    std::cin>>n>>k;
    for(long long i=0;i<n;i++) std::cin>>arr[i].first;
    for(long long i=0;i<n;i++) std::cin>>arr[i].second;
    
    sort(arr,arr+n,[&](const PII& a,const PII& b)
            {
                if(a.second!=b.second) return a.second>b.second;
                else return a.first<b.first;
            }
        );
    long long reta=0,retb=0;
    for(int i=0;i<k;i++)
    {
        reta+=arr[i].first;
        retb+=arr[i].second;
    }
    std::cout<<reta<<" "<<retb<<std::endl;


    return 0; 
}