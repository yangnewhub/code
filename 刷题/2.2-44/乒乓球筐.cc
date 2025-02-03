#include <iostream>
#include <map>
#include <string>

using namespace std;

int main() 
{
   string s1,s2;
   while(std::cin>>s1>>s2)
   {
        map<char,int> hash1,hash2;
        for(auto ch:s1)hash1[ch]++;
        for(auto ch:s2)hash2[ch]++;
        auto it = hash2.begin();
        int flag=0;
        while(it!=hash2.end())
        {
            if(hash1[it->first]<it->second)
            {
                flag=1;
                break;
            }
            it++;
        }
        if(flag==1) std::cout<<"No"<<std::endl;
        else  std::cout<<"Yes"<<std::endl;

   } 
}
// 64 位输出请用 printf("%lld")