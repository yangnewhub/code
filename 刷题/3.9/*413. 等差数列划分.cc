class Solution {
    public:
        int numberOfArithmeticSlices(vector<int>& nums) {
            //此时就是固定i，看前面两个是否和我构成等差
            //以当前位置为结尾，选，能构成的等差数列的子数组的个数
    
            int n = nums.size();
            if(n<3) return 0;
            vector<int> dp(n+1);
            int ret=0;
            for(int i=3;i<=n;i++)
            {
                if(nums[i-3]+nums[i-1]==2*nums[i-2])dp[i]=dp[i-1]+1;
                ret +=dp[i];
            }
            return ret;
        }
    };