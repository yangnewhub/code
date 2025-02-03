#include <iostream>
#include <string>
using namespace std;

int main() 
{
    string s;
    int n;
    std::cin>>s>>n;
    int index=0,num=0;
    int left=0,right=0;
    int tmp=0;
    while(right<s.size())
    {
        if(s[right]=='C'||s[right]=='G') tmp++;
        right++;
        if(right-left==n)
        {
            if(tmp>num)
            {
                num=tmp;
                index=left;
            }
            
            if(s[left]=='C'||s[left]=='G') tmp--;
            left++;
        }
    }
    std::cout<<s.substr(index,n)<<std::endl;
    return 0;

}
// 64 位输出请用 printf("%lld")