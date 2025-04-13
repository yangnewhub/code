#include <iostream>
#include <vector>
using namespace std;

vector<vector<int>> nums;
int num = 1, gan = 1;
int ret = 0;
int index = 0;
void dfs(int line)
{
    if (line == nums.size()) return;
    index = max(index, line + 1);

    for (int i = 0; i < 2; i++)
    {
        int changgan = 0;
        if (nums[line][i] == 1000 && gan < 7)
        {
            gan++;
            changgan = -1;
        }
        else if (nums[line][i] == -1000 && gan > 1)
        {
            gan--;
            changgan = 1;
        }
        else if (nums[line][i] > -1000 && nums[line][i] < 1000)
        {
            num += nums[line][i];
        }
        if (num <= 0)
        {
            num -= nums[line][i];
            continue;
        }

        ret = max(ret, gan * num);
        dfs(line + 1);

        gan += changgan;
        if (nums[line][i] > -1000 && nums[line][i] < 1000)
            num -= nums[line][i];

    }
}
int main()
{
    int n;
    cin >> n;
    vector<vector<int>> num(n, vector<int>(2));

    for (int i = 0; i < n; i++)
        for (int j = 0; j < 2; j++) cin >> num[i][j];
    nums = num;
    dfs(0);
    if (index == n) cout << 1 << " " << ret << endl;
    else cout << 0 << " " << index << endl;
}
// 64 位输出请用 printf("%lld")
// 64 位输出请用 printf("%lld")
// 64 位输出请用 printf("%lld")
//int main()
//{
//    int n, m, t;
//    cin >> n >> m >> t;
//    vector<string> names(n);
//    for (int i = 0; i < n; i++) cin >> names[i];
//    vector<vector<string>> name;
//    map<string, int> hash;
//
//    for (int i = 0; i < m; i++)
//    {
//        int t;
//        cin >> t;
//        vector<string> s(t);
//        for (int j = 0; j < t; j++)
//        {
//            cin >> s[j];
//            hash[s[j]] = i;
//        }
//        name.push_back(s);
//    }
//
//
//    vector<string> succ(t);
//    for (int i = 0; i < t; i++) cin >> succ[i];
//
//    vector<bool> check(m, false);
//
//    int ret = 0;
//    for (auto s : succ)
//    {
//        //得到第机队
//        auto it = hash.find(s);
//        if (it == hash.end()) ret++;
//        else
//        {
//            int t = it->second;
//            if (check[t] == false)
//            {
//                ret += name[t].size();
//                check[t] = true;
//            }
//        }
//    }
//    cout << ret << endl;
//    return 0;
//}
//


#include <iostream>
#include <vector>
using namespace std;

int main()
{
    int n;
    cin >> n;
    int gan = 1, num = 1;
    int index = 0;
    int ret = 0;
    for (int i = 0; i < n; i++)
    {
        index++;
        int a, b;
        cin >> a >> b;

        int achange = 0;
        if (a > 0)  achange = 1;
        else if (a < 0)achange = -1;


        int bchange = 0;
        if (b > 0)  bchange = 1;
        else if (b < 0)bchange = -1;


        //都减，优先减武器 小
        //都加  加人
        //一家一键  加
        if (achange == -1 && bchange == -1)
        {
            if (a == -1000 || b == -1000)
            {
                if (gan > 1) gan--;
            }
            else
            {
                num += max(a, b);
            }
        }
        else if (achange == 1 && bchange == 1)
        {
            if (a != 1000 && b != 1000)
            {
                num += max(a, b);
            }
            else
            {
                if (a == 1000) num += b;
                else num += a;
            }
        }
        else if (achange == 0 || bchange == 0)
        {
            if (achange == 0 && bchange == 1)
            {
                if (b == 1000)if (gan < 7) gan--;
                else num += b;
            }
            else if (bchange == 0 && achange == 1)
            {
                if (a == 1000)if (gan < 7) gan--;
                else num += a;
            }
        }
        else
        {
            if (achange == 1)
            {
                if (a == 1000 && gan < 7)gan++;
                else if (a != 1000) num += a;
            }
            else
            {
                if (b == 1000 && gan < 7)gan++;
                else if (b != 1000) num += a;
            }

        }
        ret = max(ret, gan * num);
        if (num <= 0) break;
    }

    if (index == n) cout << 1 << " " << ret << endl;
    else cout << 0 << " " << index << endl;


}