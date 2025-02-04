#include <string>
class Solution {
public:
    /**
     * 代码中的类名、方法名、参数名已经指定，请勿修改，直接返回方法规定的值即可
     *
     * 
     * @param param string字符串 
     * @return string字符串
     */
    string compressString(string param) {
        // write code here
        string ret;
        int n=param.size();
        int left=0,right=0;
        while(right<n)
        {
            ret+=param[left];
            int sum=0;
            while(param[left]==param[right])
            {
                sum++;
                right++;
            }
            if(sum!=1)
                ret+=to_string(sum);
            left=right;
        }
        return ret;
    }
};