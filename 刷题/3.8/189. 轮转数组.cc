
//三次反转
class Solution {
    public:
        void revacenum(vector<int>& nums,int i,int j)
        {
            while(i<j)
            {
                swap(nums[i++],nums[j--]);
            }
        }
        void rotate(vector<int>& nums, int k) {
            int n =nums.size();
            k%=n;
            revacenum(nums,0,n-1);
            revacenum(nums,0,k-1);
            revacenum(nums,k,n-1);
        }
    };
    // 1234567
    //4 