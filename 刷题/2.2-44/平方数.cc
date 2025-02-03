#include <iostream>
#include <math.h>
using namespace std;

int main()
{
    long long n;
    std::cin>>n;
    long long num=sqrt(n);
    int t1=n-num*num;
    int t2=(num+1)*(num+1)-n;
    if(t1>t2) num++;
    std::cout<<num*num;
    return 0;
}