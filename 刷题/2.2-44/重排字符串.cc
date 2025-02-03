#include <iostream>
#include <string>
#include <unordered_map>
#include <queue>

using namespace std;

struct cmp
{
    bool operator()(const pair<char,int>& p1,const pair<char,int>&p2)
    {
        return p1.second<p2.second;
    }
};
int main()
{
    long long  n;
    string s;
    std::cin>>n>>s;
    //存map
    unordered_map<char,long long> hash;
    for(auto ch:s) hash[ch]++;
    //存入大根堆，找到最大的次数
    priority_queue<pair<char,long long>,vector<pair<char,long long>>,cmp> q;
    long long t=0;
    auto it = hash.begin();
    while(it!=hash.end())
    {
        q.push({it->first,it->second});
        t=max(t,it->second);
        it++;
    }
    //超过n/2+n%2就no
    if(t>n/2+n%2)
    {
        std::cout<<"no"<<std::endl;
        return 0;
    }
    //此时都是yes，每次找到剩余的出现次数最多的---利用大根堆
    //此时需要保证不能重复
    string ret;
    char ch=' ';
    for(long long i=0;i<n;i++)
    {
        pair<char,long long> tmp=q.top(); 
        q.pop();
        if(tmp.first==ch)
        {
            pair<char,long long> tmp2=q.top();
            q.pop();
            ret+=tmp2.first;
            if(--tmp2.second!=0) q.push(tmp2);
            ch=tmp2.first;
            q.push(tmp);
        }
        else
        {
           
            ret+=tmp.first;
            if(--tmp.second!=0) q.push(tmp);
            ch=tmp.first;
        }
    }
    std::cout<<"yes"<<std::endl;
    std::cout<<ret<<std::endl;
    return 0;
}