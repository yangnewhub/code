class Solution {
    public:
        vector<vector<int>> merge(vector<vector<int>>& nums) {
            int n = nums.size();
            if(n==1) return nums;
            sort(nums.begin(),nums.end(),[](vector<int> a,vector<int> b){return a[0]<b[0];});
            int a = nums[0][0],b=nums[0][1];
            vector<vector<int>> ret;
            for(int i=1;i<n;i++)
            {
                if(b<nums[i][0]) 
                {
                    ret.push_back({a,b});
                    a = nums[i][0];
                    b=nums[i][1];
                }
                else  
                {
                    b = max(b,nums[i][1]);
                }
                if(i==n-1) ret.push_back({a,b});
            }
            return ret;
        }
    };