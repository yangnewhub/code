//map和逆向迭代器
class Solution {
    public:
        vector<int> maxSlidingWindow(vector<int>& nums, int k) {
            map<int,int> hash;
            int n = nums.size();
            int left=0,right=0;
            vector<int> ret;
            while(right<n)
            {
                hash[nums[right]]++;
                if(right-left+1>=k)
                {
                    auto it = hash.rbegin();
                    ret.push_back(it->first);
                    auto it2 = hash.find(nums[left]);
                    hash[it2->first]--;
                    if(hash[it2->first]==0) hash.erase(it2); 
                    left++;
                }
    
                right++;
            }
         return ret;
        }
    };