#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;

int main()
{
    int n;
    cin >> n;
    while (n--)
    {
        int m;
        cin >> m;
        vector<map<char, int>> hash(m);

        if (m == 1)
        {
            string s;
            cin >> s;
            cout << "Yes" << endl;
        }
        else
        {
            int flag = 0;
            int f = 0;
            for (int i = 1; i <= m; i++)
            {

                string s;
                cin >> s;
                if (i <= m / 2)
                {
                    for (auto x : s) hash[i][x]++;
                }
                else if (i > m / 2 + m % 2)
                {
                    for (auto x : s)
                    {
                        //这个满足
                        if (hash[m - i + 1][x] > 0)
                        {
                            flag = 1;
                            break;
                        }
                    }
                    //没有找到
                    if (flag == 0)
                    {
                        f = 1;
                    }
                }
                
            }
            if (f == 0)cout << "Yes" << endl;
            else cout << "No" << endl;
        }
    }
    return 0;
}