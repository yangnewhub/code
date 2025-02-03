class Solution {
public:
    vector<int> searchRange(vector<int>& nums, int target) {
        int n =nums.size();
        if(n==0) return {-1,-1};

        int left =0,right=n-1;
        while(left<right)
        {
            int mid = (right-left)/2+left;
            if(nums[mid]<target) left=mid+1;
            else right=mid;
        }
        int l=left;
        if(nums[left]!=target) return {-1,-1};
        left =0,right=n-1;
        while(left<right)
        {
            int mid = (right-left+1)/2+left;
            if(nums[mid]>target) right=mid-1;
            else left=mid;
        }
        int r=right;
        return {l,r};
    }
};

