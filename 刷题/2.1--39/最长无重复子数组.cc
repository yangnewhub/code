class Solution {
  public:
    /**
     * 代码中的类名、方法名、参数名已经指定，请勿修改，直接返回方法规定的值即可
     *
     *
     * @param arr int整型vector the array
     * @return int整型
     */
    int maxLength(const vector<int>& arr) {
        // write code here
        int ret = 0, n = arr.size();
        map<int, int> hash;
        int left = 0, right = 0;
        while (right < n) {
            if (hash[arr[right]] == 0) {
                hash[arr[right++]]++;
            } else {
                ret = max(ret, right - left);
                while (hash[arr[right]] > 0) {
                    hash[arr[left++]]--;
                }
            }
        }
        ret = max(ret, right - left);
        return ret;
    }

};