class Solution {
public:
    int firstUniqChar(string s) {
        map<char, int> hash;
        for (auto x : s)
        {
            hash[x]++;
        }
        for (int i = 0; i < s.size(); i++)
        {
            if (hash[s[i]] == 1) return i;
        }
        return -1;
    }
};