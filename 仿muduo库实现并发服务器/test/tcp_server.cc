#include "../source/server.hpp"
void HandleClose(Channel* channel)
{
    std::cout<<"close fd:"<<channel->Fd()<<std::endl;
    channel->Remove();
    delete channel;
}
void HandleErr(Channel* channel)
{
    return HandleClose(channel);
}
void HandleRead(Channel* channel)
{
    char date[1024]={0};
    int n=recv(channel->Fd(),date,1024,0);
    if(n<0)
    {
        ERR_LOG("read fail");
        HandleClose(channel);
    } 
    channel->EnableWrite();
    std::cout<<date<<std::endl;
}
void HandleWrite(Channel* channel)
{
    const char* date ="haha";
    int n=send(channel->Fd(),date,strlen(date),0);
    if(n<0)
    {
        ERR_LOG("write fail");
        HandleClose(channel);
    }
    channel->DisableWrite();
} 


void HandleEvent(Channel* channel)
{
    
}
void Accept( Poller* poller,Channel* channel)
{
    int newfd=accept(channel->Fd(),nullptr,nullptr);
    if(newfd < 0) {return ;}
    Channel *nchannel = new Channel(poller,newfd);
    nchannel->SetReadCallBack(std::bind(HandleRead,nchannel));
    nchannel->SetWriteCallBack(std::bind(HandleWrite,nchannel));
    nchannel->SetErrCallBack(std::bind(HandleErr,nchannel));
    nchannel->EnableRead();

}
int main()
{
    Poller poller;
    Socket listen_socfd;
    listen_socfd.CreateServerConnect(8080);
    Channel channel(&poller,listen_socfd.Sockfd());
    channel.SetReadCallBack(std::bind(Accept,&poller,&channel));
    channel.EnableRead();

    while(1)
    {
        std::vector<Channel*> active;
        poller.Poll(&active);
        for(auto ch:active)
        {
            ch->HandleEvents();
        }
    }
}