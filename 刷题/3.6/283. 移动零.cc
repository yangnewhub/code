class Solution {
    public:
        void moveZeroes(vector<int>& nums) {
            int n = nums.size();
            int left=0,right=0;
            while(right<n)
            {
                if(nums[right]!=0)
                {
                    nums[left++]=nums[right];
                }
                right++;
            }
            while(left<right)
            {
                nums[left++]=0;
            }
        }
    };