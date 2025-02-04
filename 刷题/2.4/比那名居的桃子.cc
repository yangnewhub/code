#include <iostream>
#include <vector>
using namespace std;

int main()
{
    long long n,k;
    std::cin>>n>>k;
    vector<long long> a(n),b(n);
    for(long long i=0;i<n;i++) std::cin>>a[i];
    for(long long i=0;i<n;i++) std::cin>>b[i];
    
    long long pos=0,suma=0,sumb=0;
    long long left=0,right=0;
    long long tmpa=0,tmpb=0;
    while(right<n)
    {
        tmpa+=a[right];
        tmpb+=b[right];
        right++;
        if(right-left==k)
        {
            if(tmpa>suma)
            {
                suma=tmpa;
                sumb=tmpb;
                pos=left;
            }
            else if(tmpa==suma&&tmpb<sumb)
            {
                sumb=tmpb;
                pos=left;
            }
            tmpa-=a[left];
            tmpb-=b[left];
            left++;
        }
    }
    pos++;
    std::cout<<pos<<std::endl;
    return 0;
}