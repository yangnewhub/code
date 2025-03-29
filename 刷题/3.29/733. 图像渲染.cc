class Solution {
    public:
        vector<vector<int>> floodFill(vector<vector<int>>& image, int sr, int sc, int color) {
            if (image[sr][sc]==color) return image;
    
            queue<pair<int,int>> q;
            int num = image[sr][sc];
            q.push({sr,sc});
            while(q.size())
            {
                int sz = q.size();
                auto t = q.front();
                q.pop();
                if(t.first>=0&&t.first<image.size()&&t.second>=0&&t.second<image[0].size()&&image[t.first][t.second]==num)
                {
                    image[t.first][t.second]=color;
                    q.push({t.first,t.second-1});
                    q.push({t.first,t.second+1});
                    q.push({t.first-1,t.second});
                    q.push({t.first+1,t.second});
                }
            }
    
            return image;
        }
    };