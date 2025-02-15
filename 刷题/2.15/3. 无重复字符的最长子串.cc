class Solution {
    public:
        int lengthOfLongestSubstring(string s) {
            int n=s.size();
            int left=0,right=0;
            int ret=0;
            map<char,int> hash;
            while(right<n)
            {
    
                hash[s[right]]++;
                
    
                while(hash[s[right]]>1)
                {
                    hash[s[left]]--;
                    left++;
                }
    
                right++;
                ret=max(ret,right-left);
        
            }
            return ret;
        }
    };