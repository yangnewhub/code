class Solution {
public:
    int maxDifference(string s) {
        map<char,int> hash;
        for(auto ch:s) hash[ch]++;
        int maxnum=0,minnum=100;
        for(auto p:hash)
        {
            if(p.second%2==0) minnum=min(minnum,p.second);
            else maxnum=max(maxnum,p.second);
        }
        return maxnum-minnum;
    }
};