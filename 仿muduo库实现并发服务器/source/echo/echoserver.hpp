#include "../server.hpp"
class EchoServer
{
private:
    void Connect(const PtrConnection& coon)
    {
        DBG_LOG("new connect success,%p",coon.get());
    }
    void OMessage(const PtrConnection& coon, Buffer* buff)
    {
        char date[1024]={0};
        buff->ReadAndPop(date,buff->EnableReadSize());
        DBG_LOG("%s",date);

        const char *s="nihao";
        coon->Send((char*)s,strlen(s));
        //coon->ShutDown();
    }
    void Close(const PtrConnection& coon)
    {
        DBG_LOG("now close ");
    }
public:
    EchoServer(int port)
        :_server(port)
    {
        _server.SetThreadPoolCount(2);
        _server.SetConnectCallBack(std::bind(&EchoServer::Connect,this,std::placeholders::_1));
        _server.SetMessagCallBack(std::bind(&EchoServer::OMessage,this,std::placeholders::_1,std::placeholders::_2));
        _server.SetCloseCallBack(std::bind(&EchoServer::Close,this,std::placeholders::_1));
        _server.StartTimerTask(5);
    }
    void Start()
    {
        _server.Start();
    }
private:
    TcpServer _server;
};