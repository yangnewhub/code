class Solution {
    public:
        vector<vector<int>> sortMatrix(vector<vector<int>>& grid) {
            int n=grid.size();
            vector<vector<int>> nums(n*2-1);
            for(int i=0;i<n;i++)
            {
                for(int j=0;j<n;j++)
                {
                    nums[i-j+n-1].push_back(grid[i][j]);
                }
            }
            for(int i=0;i<nums.size();i++)
            {
                if(i<n-1) sort(nums[i].begin(),nums[i].end(),[](int x,int y){return x>y;});
                else sort(nums[i].begin(),nums[i].end());
            }
            
            for(int i=0;i<n;i++)
            {
                for(int j=0;j<n;j++)
                {
                    grid[i][j]=nums[i-j+n-1].back();
                    nums[i-j+n-1].pop_back();
                }
            }
            return grid;
        }
    };