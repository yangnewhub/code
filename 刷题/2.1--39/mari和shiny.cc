#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main()
{
    long long  n;
    string s;
    std:cin >> n >> s;
    vector<long long> f(n + 1), g(n + 1),h(n+1);
    //以当前位置为结尾，有多少个s
    for (long long i = 1; i <= n; i++)
    {
        s[i - 1] == 's' ? (f[i] = f[i - 1] + 1) : (f[i] = f[i - 1]);
    }
    //以当前位置为结尾为h，有多少个sh
    for (long long i = 1; i <= n; i++)
    {
        if (s[i - 1] == 'h')
        {
            g[i] = f[i] + g[i - 1];
        }
        else g[i] = g[i - 1];
    }
    //以当前位置为结尾，有多少个shy
    for (long long i = 1; i <= n; i++)
    {
        if (s[i - 1] == 'y')
        {
            h[i] = g[i] + h[i - 1];
        }
        else h[i] = h[i - 1];
    }
    std::cout << h[n];
    return 0;
}
