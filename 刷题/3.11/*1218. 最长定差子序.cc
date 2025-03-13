//采用n^2会超时，所以列举j会超时，可以采用hash
class Solution {
    public:
        int longestSubsequence(vector<int>& arr, int difference) {
            int n = arr.size();
            unordered_map<int,int> hash;
            hash[arr[0]]++;
            int ret = 1;
            for(int i =1;i<n;i++)
            {
                int num = arr[i] - difference;
                hash[arr[i]]=hash[num]+1;
                ret = max (ret,hash[arr[i]]);
            }
            return ret;
    
        }
    };