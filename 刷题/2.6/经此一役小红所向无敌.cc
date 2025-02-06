#include <iostream>
using namespace std;

int main()
{
    int a,h,b,k;
    cin>>a>>h>>b>>k;
    long long ret=0;
//     //保证a是最大的
//     if(a<b)
//     {
//         swap(a,b);
//         swap(h,k);
//     }
    //进行计算
    while(h>0&&k>0)
    {
        ret+=a+b;
        h-=b;
        k-=a;
    }
    if(h>0) ret+=10*a;
    if(k>0) ret+=10*b;
    cout<<ret<<endl;
    return 0;
}