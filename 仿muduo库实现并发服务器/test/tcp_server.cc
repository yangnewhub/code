#include "../source/server.hpp"


void HandleRead(Channel* channel)
{

}
void HandleWrite(Channel* channel)
{
    
}
void HandleErr(Channel* channel)
{
    
}
void HandleClose(Channel* channel)
{
    
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
    Socket s;
    s.CreateServerConnect(8080);
    Channel channel(&poller,s.Sockfd());
    channel.SetReadCallBack(std::bind(Accept,&poller,&channel));
    while(1)
    {
        int nfd=s.Accept();
        if(nfd<0)
        {
            continue;
        }
        Socket client_fd(nfd);
        char buffer[1024]={0};
        int n= client_fd.Recv(buffer,1024);
        if(n<0)
        {
            client_fd.Close();
            continue;
        }
        n=client_fd.Send(buffer,1024);

        
    }
}