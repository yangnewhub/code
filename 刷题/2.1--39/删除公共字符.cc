#include <iostream>
#include <string>
#include <map>
using namespace std;

int main() 
{
    string s1,s2;
    getline(std::cin,s1);
    getline(std::cin,s2);
    map<char,int> hash;
    for(auto s:s2)
    {
        hash[s]++;
    }
    for(auto s:s1)
    {
        if(hash[s]==0)
        {
            std::cout<<s;
        }
    }

}
// 64 位输出请用 printf("%lld")