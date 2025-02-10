
class Solution {
    public:
        int evalRPN(const vector<string>& tokens) {
            map<string, function<int(int, int)>> hash = {
                {"+",[](int x,int y) {return x + y;}},
                {"-",[](int x,int y) {return x - y;}},
                {"*",[](int x,int y) {return x * y;}},
                {"/",[](int x,int y) {return x / y;}}
    
            };
            stack<int> s;
            for (auto x : tokens)
            {
                if (x == "+"  ||x == "-"||x=="*"||x=="/")
                {
                    int y = s.top();
                    s.pop();
                    int X = s.top();
                    s.pop();
                    int ret = hash[x](X, y);
                    s.push(ret);
                }
                else
                {
                    s.push(stoi(x));
    
    
                }
            }
            return s.top();
        }
    };