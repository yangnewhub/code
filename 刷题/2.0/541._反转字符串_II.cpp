#include <iostream>
#include <string>
using namespace std;

class Solution {
public:
    void myreverse(string& s, int i, int j)
    {
        while (i < j)
        {
            swap(s[i++], s[j--]);
        }
    }

    string reverseStr(string s, int k) {
        int n = s.size();
        int count = 0;
        int begin = 0;
        for (int i = 0; i < n; i++)
        {
            count++;
            if (i == n - 1)
            {
                if (count < k) myreverse(s, begin, i);
                else myreverse(s, begin, begin + k - 1);
            }
            if (count == 2 * k)
            {
                myreverse(s, begin, begin + k - 1);
                count = 0;
                begin = i + 1;
            }
        }
        return s;
    }
};

int main()
{
    Solution sou;
    string s = "abcd";
    sou.reverseStr(s, 2);
}