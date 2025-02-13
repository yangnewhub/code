class Solution {
    public:
        int findUnsortedSubarray(vector<int>& nums) {
            int n=nums.size();
            if(n==1) return 0;
            vector<int> numsa=nums;
            sort(numsa.begin(),numsa.end());
            int left=0;
            while(left!=n-1&&nums[left]==numsa[left]) left++;
            int right=n-1;
            while(right!=0&&nums[right]==numsa[right])right--;
            return right>left?right-left+1:0;
        }
    };