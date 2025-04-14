class Solution {
    private:
        int n,m;
        vector<int> ret;
        int dx[4]={0,1,0,-1};
        int dy[4]={1,0,-1,0};
        vector<vector<bool>> check;
        int t=0;
    public:
        vector<int> spiralOrder(vector<vector<int>>& matrix) {
            n=matrix.size();
            m=matrix[0].size();
            check.resize(n+2,vector<bool>(m+2));
            for(int i =0;i<m+2;i++) check[0][i]=true;
            for(int i =0;i<m+2;i++) check[n+1][i]=true;
            for(int i =0;i<n+2;i++) check[i][0]=true;
            for(int i =0;i<n+2;i++) check[i][m+1]=true;
    
            dfs(matrix,0,0);
            return ret;
        }
        void dfs(vector<vector<int>>& matrix,int i,int j)
        {
            if(check[i+1][j+1]==true) return ;
            ret.push_back(matrix[i][j]);
            check[i+1][j+1]=true;
            int x = i+dx[t];
            int y = j + dy[t];
            if(check[x+1][y+1])
            {
                t=(t+1)%4;
                x = i+dx[t];
                y = j + dy[t];
            }
            dfs(matrix,x,y);
    
    
        }
    };