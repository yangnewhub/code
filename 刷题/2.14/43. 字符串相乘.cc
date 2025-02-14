class Solution {
    public:
        string multiply( string num1,  string num2) {
            int n = num1.size(), m = num2.size();
            vector<vector<int>> nums(m, vector<int>(n + m - 1));
            vector<int> ret(n + m);
            reverse(num2.begin(), num2.end());
            reverse(num1.begin(), num1.end());
            int count = 0;
            for (int i = 0;i < m;i++)
            {
                for (int j = 0;j < n;j++)
                {
                    nums[i][j + count] = (num1[j] - '0') * (num2[i] - '0');
                    ret[j + count] += nums[i][j + count];
                }
                count++;
            }
            int t = 0;
            for (int i = 0;i < ret.size();i++)
            {
                    int sum = ret[i] + t;
                     t = sum / 10;
                     ret[i] = sum % 10;
    
                
            }
            int pos=ret.size()-1;
            while(pos>0&&ret[pos]==0) 
            {
                ret.pop_back();
                pos--;
            }
            reverse(ret.begin(), ret.end());
            string s;
    
            for (auto x : ret)
            {
                s += to_string(x);
            }
            return s;
    
        }
    };