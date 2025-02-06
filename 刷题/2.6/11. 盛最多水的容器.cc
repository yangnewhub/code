class Solution {
public:
    int maxArea(vector<int>& height) {
        int n =height.size();
        int left=0,right=n-1;
        int ret=0;
        while(left<right)
        {
            int t=(right-left)*min(height[left],height[right]);
            if(height[left]<height[right]) left++;
            else right--;
            ret =max(ret,t);
        }
        return ret;
    }
};