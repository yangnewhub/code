class Solution {
    public:
        int longestConsecutive(vector<int>& nums) {
          sort(nums.begin(),nums.end());
          int n = nums.size();
          if(n==0||n==1)return n;
          int left = 0 ,right = 1;
          int ret =1,tmp = 1;
          while(right < n)
          {
            if(nums[right]==nums[right-1]+1)
            {
                tmp+=1;
                ret = max(ret,tmp);
                
            }
            else if(nums[right]==nums[right-1])
            {
                right++;
                continue;
            }
            else 
            {
                tmp=1;
                left=right;
                right;
            }
            right++;
          }  
          return ret;
        }
    };