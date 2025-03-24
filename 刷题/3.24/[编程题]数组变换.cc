#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    int n ;
    cin>>n;
    vector<int> nums(n);
    for(int i =0;i<n;i++)cin>>nums[i];
    sort(nums.begin(),nums.end());
    if(nums[0]==nums[n-1])
    {
        cout<<"YES"<<endl;
        return 0;
    }
    for(int i=0;i<n-1;i++)
    {
        if(nums[n-1]%nums[i]!=0||(nums[n-1]/nums[i])%2!=0)
        {
            cout<<"NO"<<endl;
            return 0;
        }
    }
    cout<<"YES"<<endl;
    return 0;
}
// 64 位输出请用 printf("%lld")