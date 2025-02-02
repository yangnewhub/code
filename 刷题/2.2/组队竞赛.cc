#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() 
{
    int n;
    std::cin>>n;
    vector<int> nums(3*n);
    for(int i=0;i<3*n;i++)
    {
        std::cin>>nums[i];
    }
    sort(nums.begin(),nums.end());
    long long ret=0;
    int left=0,right=3*n-1;
    while(left<right)
    {
        left++;
        right--;
        ret+=nums[right--];
    }
    std::cout<<ret<<std::endl;
    return 0;
}
// 64 位输出请用 printf("%lld")