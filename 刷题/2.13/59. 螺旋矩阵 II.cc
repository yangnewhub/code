class Solution {
    public:
        vector<vector<int>> generateMatrix(int n) {
            int X[4]={0,1,0,-1},Y[4]={1,0,-1,0};
            vector<vector<int>> nums(n,vector<int>(n));
            vector<vector<bool>> check(n+2,vector<bool>(n+2,false));
            for(int i=1;i<=n;i++)
            {
                for(int j=1;j<=n;j++) 
                    check[i][j]=true;
            }
            int x=0,y=0;
            int index=0;
            if(n%2==0)index=n/2-1;
            else index=n/2;
            int num=1,i=0;
            while(x!=n/2||y!=index)
            {
                nums[x][y]=num++; 
                check[1+x][1+y]=false;
                if(check[1+x+X[i]][1+y+Y[i]]==false) i=(i+1)%4;
                x+=X[i];
                y+=Y[i];
            }
            nums[x][y]=num;
            return nums;
    
        }
    };