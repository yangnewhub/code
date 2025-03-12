//固定长度的滑动窗口
//存在一些问题
class Solution {
    public:
        int countOfSubstrings(string word, int k) {
            int n = word.size();
            int ret = 0 ,sum=0;
            int left =0,right=0;
            map<char,int> hash;
            while(right < n)
            {
                if(word[right]=='a'||word[right]=='e'||word[right]=='i'||word[right]=='o'||
                word[right]=='u') 
                {
                    hash[word[right]]++;
                    sum++;
                }
    
                if(right-left+1 == 5+k)
                {
                    if(sum==5) ret++;
                    if(hash[word[left]]) 
                    {
                        hash[word[left]]--;
                        sum--;
                    }
                    left++;
                }  
                right++;
            }
            return ret;
        }
    };