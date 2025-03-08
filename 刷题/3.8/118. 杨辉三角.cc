class Solution {
    public:
        vector<vector<int>> generate(int numRows) {
            vector<vector<int>> ret;
            ret.push_back({1});
            if(numRows==1) return ret;
    
            ret.push_back({1,1});
            if(numRows==2)return ret;
            vector<int> nums(2,1);
            for(int i=3;i<=numRows;i++)
            {
                nums.resize(i);
                nums[i-1]=1;
                for(int j=i-2;j>=1;j--)
                {
                    nums[j]=nums[j-1]+nums[j];
                }
                ret.push_back(nums);
            }
            return ret;
            
    
        }
    };