class Solution {
    public:
        bool hasSameDigits(string s) {
            int n=s.size();
            dfs(s);
            return s[0]==s[1];
        }
        void dfs(string& s)
        {
            int n=s.size();
            if(n==2) return;
            int prev=0,cur=1;
            while(cur<n)
            {
                s[prev]=(s[prev]-'0'+s[cur]-'0')%10+'0';
                prev++;
                cur++;
            }
            s.pop_back();
            dfs(s);
        }
    };