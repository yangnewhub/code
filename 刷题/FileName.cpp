#include <iostream>
#include <string>
#include <cstring>

using namespace std;

bool solve(string A, string B) {
    // write code here
    if (A == B) return true;
    int n = A.size();
    int m = B.size();
    int pos = A.find(B[0]);
    while (pos != string::npos)
    { 

        if ((strncmp(A.c_str() + pos, B.c_str(), n-pos) == 0) && (strncmp(A.c_str(), B.c_str() + n-pos, pos)==0))
        {
           
            return true;
        }
        pos = A.find( B[0],pos+1);
    }
    return false;
}
int main()
{
     string A = "nowncoder";
    string B = "ncodernow";
    solve(A, B);
}