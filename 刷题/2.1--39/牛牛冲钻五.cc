#include<iostream>
#include<string>
using namespace std;

int main()
{
    int n;
    std::cin>>n;
    while(n--)
    {
        int a,b,ret=0,tmp=0;
        string s;
        std::cin>>a>>b>>s;
        for(auto x:s)
        {
            if(x=='W')
            {
                ret++;
                tmp++;
                if(tmp>=3)
                {
                    ret+=b-1;
                }
            }
            else
            {
                ret--;
                tmp=0;
            }
        }
        std::cout<<ret<<std::endl;
    }
    return 0;
}
