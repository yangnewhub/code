#include "../source/server.hpp"
#include <time.h>

// void HandleClose(Channel* channel)
// {
//     DBG_LOG("close fd: %d",channel->Fd());
//     channel->Remove();
//     delete channel;
// }
// void HandleErr(Channel* channel)
// {
//     return HandleClose(channel);
// }
// void HandleRead(Channel* channel)
// {
//     char date[1024]={0};
//     int n=recv(channel->Fd(),date,1024,0);
//     if(n<0)
//     {
//         ERR_LOG("read fail");
//         HandleClose(channel);
//     } 
//     channel->EnableWrite();
//     DBG_LOG("recv success date:%s",date);
// }
// void HandleWrite(Channel* channel)
// {
//     const char* date ="haha";
//     int n=send(channel->Fd(),date,strlen(date),0);
//     if(n<0)
//     {
//         ERR_LOG("write fail");
//         HandleClose(channel);
//     }
//     channel->DisableWrite();
// } 


// void HandleEvent(Channel* channel,EventLoop* loop,int id)
// {
//     loop->TimerReferesh(id);
// }
// ConnectCallBack _connect_callback;
// MessagCallBack _message_callback;
// CloseCallBack _close_callback;
// EventCallBack _event_callback;
//CloseCallBack _server_close_callback;   
using PtrConnection=std::shared_ptr<Connection>;
std::unordered_map<int,PtrConnection> conns;
int times = rand()%10000;
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
    coon->ShutDown();
}
void Close(const PtrConnection& coon)
{
    DBG_LOG("now close ");
}
void ServeClose(const PtrConnection& coon)
{
    conns.erase(times);
}
void Accept( EventLoop* loop,Channel* channel)
{
    int newfd=accept(channel->Fd(),nullptr,nullptr);
    if(newfd < 0) {return ;}
    

    PtrConnection conn (new Connection(loop,times,newfd));
    conns[times]=conn;
    conn->SetConnectCallBack(std::bind(Connect,std::placeholders::_1));
    conn->SetMessagCallBack(std::bind(OMessage,std::placeholders::_1,std::placeholders::_2));
    conn->SetServeCloseCallBack(std::bind(ServeClose,std::placeholders::_1));
    conn->SetCloseCallBack(std::bind(Close,std::placeholders::_1));

    conn->StartTimerTask(10);
    conn->Connect();
    
    // nchannel->SetReadCallBack(std::bind(HandleRead,nchannel));
    // nchannel->SetWriteCallBack(std::bind(HandleWrite,nchannel));
    // nchannel->SetErrCallBack(std::bind(HandleErr,nchannel));
    // nchannel->SetEventsCallBack(std::bind(HandleEvent,nchannel,loop,times));
    // //定时任务，10秒后没有没有反应就关闭
    // loop->TimerAdd(times,std::bind(&HandleClose,nchannel),10);
    // nchannel->EnableRead();

}
int main()
{
    srand(time(NULL));

    EventLoop loop;
    Socket listen_socfd;
    listen_socfd.CreateServerConnect(8081);
    Channel channel(&loop,listen_socfd.Sockfd());
    channel.SetReadCallBack(std::bind(&Accept,&loop,&channel));
    channel.EnableRead();   
  
    loop.Start();
}