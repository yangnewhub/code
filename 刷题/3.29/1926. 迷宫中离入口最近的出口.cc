class Solution {
    public:
        int nearestExit(vector<vector<char>>& maze, vector<int>& entrance) {
            queue<pair<int,int>> q;
            int n = maze.size(),m = maze[0].size();
            vector<vector<bool>> check(n,vector<bool>(m));
            q.push({entrance[0],entrance[1]});
            check[entrance[0]][entrance[1]] = true;
            int dx[4]={0,0,-1,1};
            int dy[4]={-1,1,0,0};
            int ret=0;
            while(q.size())
            {
                int sz = q.size();
                ret++;
                for(int i=0;i<sz;i++)
                {
                    auto [a,b] = q.front();
                    q.pop();
                    for(int i=0;i<4;i++)
                    {
                        int x = a+dx[i];
                        int y = b+dy[i];
                        if(x>=0&&x<n&&y>=0&&y<m&&maze[x][y]=='.'&&check[x][y]==false)
                        {
                            q.push({x,y});
                            check[x][y]=true;
                            if(x==0||x==n-1||y==0||y==m-1) return ret;
                        }
                        
                    }
                }
            }
    
            return -1;
        }
    };