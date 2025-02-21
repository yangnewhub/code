#include "server.hpp"

int main()
{
    Buffer buffer;
    std::string str="hello!!!";
    buffer.WriteStringAndPush(str);
    // std::vector<char> b=buffer.Buffers();
    // for(auto x:b)
    // {
    //     std::cout<<x;
    // }


    // char* date;
    // buffer.Read(date,buffer.EnableReadSize());
    // std::cout<<date<<std::endl;
    
    // char* date;
    // buffer.ReadAndPop(date,buffer.EnableReadSize());
    // std::cout<<date<<std::endl;

    std::string tmp=buffer.ReadStringPop(buffer.EnableReadSize());
    std::cout<<tmp<<std::endl;


    Socket s;
    s.CreateServerConnect(8888);
    
    sleep(10);
    return 0;
}