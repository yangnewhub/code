class Solution {
    public:
        int countPrefixes(vector<string>& words, string s) {
            int n = s.size();
            int ret=0;
            for(auto str:words)
            {
                if(s.substr(0,str.size())==str) ret++;
            }
            return ret;
        }
    };