class StringFormat {
public:
    string formatString(string A, int n, vector<char> arg, int m) {
        // write code here
        size_t pos=A.find("%s");
        int i=0;
        while(pos!=string::npos)
        {
            A.erase(pos,2);
            A.insert(pos,1,arg[i++]);
            pos=A.find("%s");
        }
        while(i<m)
        {
            A+=arg[i++];
        }
        return A;
    }
};