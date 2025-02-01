#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int n =prices.size();
        vector<int> f(n+1),g(n+1);
        f[1]=-1*prices[0];
        
        for(int i=2;i<=n;i++)
        {
            f[i]=max(g[i-1]-prices[i-1],f[i-1]);
            g[i]=max(g[i-1],f[i-1]+prices[i-1]);
        }
        return g[n];
    }
};