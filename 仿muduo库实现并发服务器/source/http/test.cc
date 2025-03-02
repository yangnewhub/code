#include "http.hpp"
Util u;
void test1()
{
    std::vector<std::string> ret;
    u.Spilt("aaa.bbb..vvv..",".",&ret);
    for(auto x:ret)
    {
        std::cout<<x<<std::endl;
    }
}
void test2()
{
    char*buf;
    u.ReadFile("./http.hpp",buf);
    std::cout<<buf<<std::endl;
}
void test3()
{
    char* buf;
    u.ReadFile("http.hpp",buf);
    u.WriteFile("a.hpp",buf);
}
void test4()
{
    std::cout<<u.EnCode("abc123++ddd-2_",false)<<std::endl;
    std::cout<<u.EnCode("abc123++ddd-2_",true)<<std::endl;

    std::cout<<u.DeCode("abc123%2B%2Bddd-2_",false)<<std::endl;
    std::cout<<u.DeCode("abc123  ddd-2_",true)<<std::endl;
}
void test5()
{
    std::cout<<u.PathIsVaile("/a/b/../../..")<<std::endl;
    std::cout<<u.PathIsVaile("/a/b/../../c")<<std::endl;
    std::cout<<u.PathIsVaile("/a/b/../c/..")<<std::endl;
    std::cout<<u.PathIsVaile("/a/b/c/../..")<<std::endl;

}
void test6()
{
    HttpRequest request;
}
int main()
{
    test6();
    
}