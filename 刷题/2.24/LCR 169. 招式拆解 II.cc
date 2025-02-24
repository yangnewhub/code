class Solution {
    public:
        char dismantlingAction(string arr) {
            map<char,int> hash;
            for(auto x:arr)hash[x]++;
            for(auto x:arr)
                if(hash[x]==1)
                {
                    return x;
                }
            return ' ';
        }
    };