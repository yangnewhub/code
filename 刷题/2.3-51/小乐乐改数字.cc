#include <iostream>
#include <string>
using namespace std;

int main() 
{
   string s;
   std::cin>>s;
   for(int i=0;i<s.size();i++)
   {
        if((s[i]-'1'+1)%2==0)s[i]='0';
        else s[i]='1';
   }
   
   int pos=0;
   for(int i=0;i<s.size();i++)
   {
        if(s[i]=='0')
        {
            pos++;
        }
        else break;
   }
   std::cout<<s.substr(pos);
}
// 64 位输出请用 printf("%lld")