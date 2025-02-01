#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main()
{
    string s;
    getline(std::cin, s);
    vector<string> nums;
    size_t pos = s.find(" ");
    while (pos!= string::npos)
    {
        nums.push_back(s.substr(0, pos));
        s.erase(0, pos + 1);
        pos = s.find(" ");
    }
    nums.push_back(s);
    for (int i = nums.size() - 1; i >= 0; i--)
    {
        std::cout << nums[i] << " ";
    }
}
// 64 位输出请用 printf("%lld")