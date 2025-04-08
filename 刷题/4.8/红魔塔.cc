#include <iostream>
using namespace std;

int main()
{
    int x,y,z;
    cin>>x>>y>>z;
    int t = z/y;
    if(t>=x) cout<<x*y;
    else if(t<x) cout<<z;
    return 0;
}