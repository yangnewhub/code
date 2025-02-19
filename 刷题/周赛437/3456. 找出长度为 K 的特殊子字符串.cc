class Solution {
    public:
        bool hasSpecialSubstring(string s, int k) {
            int n=s.size();
            int left=0,right=0;
            map<char,int> hash;
           
    
            while(right<n)
            {
                //入窗口
                hash[s[right]]++;
                //判断
                if(right-left+1==k)
                {
                    if(hash[s[right]]==k)
                    {
                        int r=0;
                        if((left!=0&&s[left]!=s[left-1])||left==0) r++;
                        if((right!=n-1&&s[right]!=s[right+1])||right==n-1) r++;
                        if(r==2) return true;
                    }
                   //出窗口
                   hash[s[left]]--;
                   left++;
                }
                right++;
            }
            return false;
        }
    };