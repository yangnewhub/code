class Solution {
    public:
        bool checkValidString(string s) {
            /*
                lower剩余最小的（的个数
                upper剩余最大的（的个数
            */
            int lower=0,upper=0;
            for(auto ch:s)
            {
                if(ch=='(')
                {
                    lower++;
                    upper++;
                }
                else if(ch==')')
                {
                    lower--;
                    upper--;
                }
                else
                {
                    lower--;
                    upper++;
                }
                if(upper<0) return false;
                if(lower<0)lower++;  
                
            }
            return lower==0;
        }
    };