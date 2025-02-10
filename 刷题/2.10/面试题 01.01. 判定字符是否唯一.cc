class Solution {
    public:
        bool isUnique(string astr) {
            map<char,int> hash;
            for(auto x:astr) {
                if(hash[x]!=0) return false;
                hash[x]++;
            }
            return true;
        }
    };