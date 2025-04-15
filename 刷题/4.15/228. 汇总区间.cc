class Solution {
    public:
        vector<string> summaryRanges(vector<int>& nums) {
            vector<string> ret;
            int n = nums.size();
            int left =0,right=1;
            while(right<n)
            {
                string s;
                s+=to_string(nums[left]);
                while(right<n&&nums[right]==nums[right-1]+1) right++;
                //4 5 6 8
                if(right!=left+1) s+="->"+to_string(nums[right-1]);
    
                left=right;
                right++;
                ret.push_back(s);
            }
            if(left == n-1) ret.push_back(to_string(nums[left]));
            return ret;
        }
    };