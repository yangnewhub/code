class Solution {
    public:
        vector<int> minCosts(vector<int>& cost) {
            int n = cost.size();
            vector<int> nums(n);
            int tmp = cost[0];
            for(int i = 0 ;i<n ; i++)
            {
                tmp = min(tmp,cost[i]);
                nums[i]=tmp;
            }
            return nums;
        }
    };