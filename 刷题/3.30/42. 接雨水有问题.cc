class Solution {
    public:
        int trap(vector<int>& nums) {
            int n = nums.size();
            int left = 0, right = 1;
            int ret=0;
            while(left<n)
            {
                right=left+1;
                int sum=0;
                while(right<n)
                {
                    if(nums[left]<= nums[right])break;
                    sum+=nums[right++];
                }
                if(nums[right]>=nums[left]) 
                {
                    ret+=nums[left]*(right-left-1)-sum;
                    left=right;
                }
                else left++;
            }
            return ret;
        }
    };