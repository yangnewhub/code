#include <iostream>
#include <vector>
#include <math.h>
using namespace std;
const long long NUM = 1e7 + 9;
int main()
{
    int n, d, m;
    cin >> n >> d >> m;
    vector<vector<int>> nums(n, vector<int>(d + 1));
    for (int i = 0; i < n; i++)
    {
        cin >> nums[i][0];
        for (int j = 1; j <= nums[i][0] + 1; j++)
            cin >> nums[i][j];
    }



    for (int a = 0; a < m; a++)
    {
        int p, l, r;
        cin >> p >> l >> r;
        vector<int> num(d + 1);
        for (int i = 1; i <= 1 + p; i++) cin >> num[i];

        //for(auto x: num) cout<<x;
        for (int i = l - 1; i < r; i++)
        {
            if (a % 2 == 0)
            {
                for (int j = 1; j < d + 1; j++)
                {
                    nums[i][j] -= num[j];
                }
            }
            else
            {
                for (int j = 1; j < d + 1; j++)
                {
                    nums[i][j] += num[j];
                }
            }
        }
    }


    for (int i = 0; i < n; i++)
    {
        long long ret = nums[i][1];
        for (int j = 2; j < d + 1; j++)
        {
            long long x = nums[i][j];
            for (int o = 0; o < j - 1; o++)
            {
                x = (x * (long long)233) % NUM;
            }
            ret += x;
            ret %= NUM;
        }


        cout << ret << " ";
    }

    return 0;
}
// 64 位输出请用 printf("%lld")