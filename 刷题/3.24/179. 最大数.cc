class Solution {
    public:
        string largestNumber(vector<int>& nums) {
            int n = nums.size();
            vector<string> num;
            for(auto x:nums) num.push_back(to_string(x));
    
            sort(num.begin(),num.end(),[](const string s1,const string s2)
                    {
                        return s1+s2>s2+s1;
                    }
                );
            string ret;
            int flag=0;
            for(auto s:num) if(s!="0") flag=1;
            if(flag==0) return "0";
            for(auto s:num) ret+=s;
            return ret;
        }
    };