#include <iostream>
using namespace std;

int main() 
{
   int n,a;
   while(cin>>n>>a)
   {
        int b;
        while(n--)
        {
            cin>>b;
            if(b<=a)a+=b;
            else
            {
                int t =a,m;
                while(t%b)
                {
                    t=t%b;
                    swap(t,b);

                }
                a+=b;
            }
            
        }
        std::cout<<a<<std::endl;
   }
}
// 64 位输出请用 printf("%lld")