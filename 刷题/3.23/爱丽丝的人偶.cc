#include <iostream>

using namespace std;

int main()
{
    int n;
    cin>>n;
    int left=1,right=n;
    while(left<right)
    {
        std::cout<<left++<<" "<<right--<<" ";
    }
    if(left==right) std::cout<<left;
    
}