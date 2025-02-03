class Solution {
public:
    int lengthOfLastWord(string s) {
        int n =s.size();
        int pos = n-1;
        while(s[pos]==' ')
        {
            pos--;
        }
        int ret=0;
        while(pos!=0&&s[pos]!=' ')
        {
            ret++;
            pos--;
        }
        if(s[pos]!=' ')ret++;
        return ret;
    }
};