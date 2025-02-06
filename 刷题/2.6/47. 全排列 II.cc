class Solution {
private:
    int n;
    vector<bool> check;
    vector<vector<int>> ret;
    vector<int> path;
public:
    void dfs(vector<int>& nums)
    {
        if(path.size()==n)
        {
            ret.push_back(path);
            return;
        }


        map<int,int> hash;
        for(int i=0;i<n;i++)
        {
            if(check[i]==false&&hash[nums[i]]==0)
            {
                path.push_back(nums[i]);
                hash[nums[i]]++;
                check[i]=true;

                dfs(nums);
                
                path.pop_back();
                check[i]=false;
            }
        }
    }
    vector<vector<int>> permuteUnique(vector<int>& nums) {
        n=nums.size();
        sort(nums.begin(),nums.end());
        check.resize(n,false);

        dfs(nums);
        return ret;
        
    }
};