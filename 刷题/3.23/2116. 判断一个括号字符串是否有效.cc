class Solution {
    public:
        bool canBeValid(string s, string locked) {
            /*
                lower 最小的为被匹配的（
                upper 最打的没被匹配的（
            */
    
            int lower=0,upper=0;
            int n =s.size();
            if(n%2==1) return false;
            for(int i=0;i<n;i++)
            {
                if(locked[i]=='1')
                {
                    if(s[i]=='(')
                    {
                        lower++;
                        upper++;
                    }
                    else
                    {
                        lower--;
                        upper--;
                    }
                }
                else 
                {
                    lower--;
                    upper++;
                }
                //说明全为（还不够  --false
                if(upper<0) return false;
                //全都是），（不够了，需要把一个）变成（
                if(lower<0) lower+=2;
        
            }
            return lower==0;
        }
    };