#include <iostream>
#include <vector>
#include<map>
using namespace std;

class Solution {
private:
    vector<string> nums;
    int ret = 0;
public:


    int longestSubstring(string s, int k)
    {
        SplitString(s, k);
        return ret;

    }
    bool istrue(string s, int k)
    {
        map<char, int> hash;

        for (auto x : s)
        {
            hash[x]++;
        }
        for (int i = 0; i < s.size(); i++)
        {
            if (hash[s[i]] < k) 
                return false;
        }
        return true;
    }
    void SplitString(string s, int k)
    {
        if (s.size() < k) return;

        int n = s.size();
        map<char, int> hash;

        for (auto x : s)
        {
            hash[x]++;
        }
        

        int index = 0;

        //确定初始的位置
        while (true)
        {
            if (index<s.size()&&hash[s[index]] < k) index++;
            else break;
        }

        //分割
         for (int i = index; i < n; i++)
        { 
            if (hash[s[i]] < k)
            {
                string tmp = s.substr(index, i-index);
                if (istrue(tmp, k))
                {
                    nums.push_back(tmp);

                    ret = max(ret, (int)tmp.size());
                    index = i + 1;
                }
                else
                {
                    SplitString(tmp, k);
                }

            }
            if (i == n - 1 && hash[s[i]] >= k)
            {
                string tmp = s.substr(index, i - index+1);
                if (istrue(tmp, k))
                {
                    nums.push_back(tmp);
                    ret = max(ret, (int)tmp.size());
                    index = i + 1;
                }
                else
                {
                    SplitString(tmp, k);
                }
            }
         }
    }
};

int main()
{
    Solution ss;
    string s = "ababacb";
    ss.longestSubstring(s, 3);
}