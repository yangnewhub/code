//79
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <queue>
#include <functional>
using namespace std;

int main()
{
    long long n, m;
    cin >> n >> m;
    string s;
    cin >> s;
    vector<long long> diff(n - 1);
    vector<long long>nums(n);
    for (long long i = 0; i < n; i++)cin >> nums[i];
    for (long long i = 0; i < n - 1; i++)diff[i] = s[i] - s[i + 1];
    vector<long long>num;
    long long index = 0, enage = 0;
    while (enage <= m && index < n - 1)
    {
        //先找到前面，<0的
        if (diff[index] < 0 && enage + nums[index] <= m)
        {
            num.push_back(index);
            enage += nums[index];
            long long i = index - 1;
            //针对和它相等的
            while (i >= 0 && diff[i] == 0)i--;
            i++;
            std::priority_queue<int, std::vector<int>, std::function<bool(int, int)>> qu([](int a, int b) { return a > b; });
            while (i < index) qu.push(nums[i++]);
            i--;
            //先找小的
            while (i >= 0 && !qu.empty())
            {
                if (enage + qu.top() > m) break;
                enage += qu.top();
                num.push_back(i);
                i--;
                qu.pop();
            }

        }
        index++;
    }

    sort(num.begin(), num.end());
    string ret;
    long long j = 0;
    for (long long i = 0; i < n; i++)
    {

        if (j < num.size() && i == num[j])
        {
            ret += s[i];
            ret += s[i];
            j++;
        }
        else ret += s[i];
    }
    cout << ret;
    return 0;
}