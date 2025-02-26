#include "../source/server.hpp"

int main()
{
    Socket client;
    client.CreateClientConnect("127.0.0.1",8081);
    int cut=5;
    while(1)
    {
        std::string date="今天天气不错";
        client.Send((void*)date.c_str(),date.size(),0);
        DBG_LOG("send success");
        
        char buffer[1024]={0};
        client.Recv(buffer,1023,0);
        std::cout<<buffer<<std::endl;
        cut--;
        if(cut==0) sleep(100);
        else sleep(1);
    }
}