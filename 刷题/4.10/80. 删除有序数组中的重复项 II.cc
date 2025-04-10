class Solution {
    public:
        int removeDuplicates(vector<int>& nums) {
            int n = nums.size();
    
            int count = 0,tmp=nums[0];
            int left =0,right=0;
            while(right < n)
            {
             
                if(tmp==nums[right])
                {
                    count++;
                    nums[left++] = nums[right++];
                    if(count==2)
                    {
                        while(right<n&&nums[right]==tmp) right++;
                        if(right<n)
                        tmp = nums[right];
                        count=0;
                    }
                }
                else 
                {
                    tmp = nums[right];
                    nums[left++] = nums[right++];
                    count=1;
    
                }
               
            }
                return left;
    
        }
    };