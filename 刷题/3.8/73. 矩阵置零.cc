//模拟
class Solution {
    public:
        void setZeroes(vector<vector<int>>& matrix) {
            int n =matrix.size(),m = matrix[0].size();
            set<int> hashx,hashy;
            for(int i=0;i<n;i++)
            {
                for(int j =0;j<m;j++)
                {
                    if(matrix[i][j]==0)
                    {
                        hashx.insert(i);
                        hashy.insert(j);
                    }
                }
            }
            for(auto x: hashx)
            {
                for(int i=0;i<m;i++) matrix[x][i]=0;
            }
             for(auto y: hashy)
            {
                for(int i=0;i<n;i++) matrix[i][y]=0;
            }
        }
    };