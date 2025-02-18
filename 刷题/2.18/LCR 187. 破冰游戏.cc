class Solution {
    int f(int num, int target) {
        if (num == 1) {
            return 0;
        }
        int x = f(num - 1, target);
        return (target + x) % num;
    }
public:
    int iceBreakingGame(int num, int target) {
        return f(num, target);
    }
};
