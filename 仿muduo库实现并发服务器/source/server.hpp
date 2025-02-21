#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <assert.h>
#include <memory>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <functional>
#include <sys/epoll.h>
#include <unordered_map>

#define INF 0
#define DBG 1
#define ERR 2
#define LOG_LEVEL INF

#define LOG(level, format, ...) do{\
        if (level < LOG_LEVEL) break;\
        time_t t = time(NULL);\
        struct tm *ltm = localtime(&t);\
        char tmp[32] = {0};\
        strftime(tmp, 31, "%H:%M:%S", ltm);\
        fprintf(stdout, "[%p %s %s:%d] " format "\n", (void*)pthread_self(), tmp, __FILE__, __LINE__, ##__VA_ARGS__);\
    }while(0)

#define INF_LOG(format, ...) LOG(INF, format, ##__VA_ARGS__)
#define DBG_LOG(format, ...) LOG(DBG, format, ##__VA_ARGS__)
#define ERR_LOG(format, ...) LOG(ERR, format, ##__VA_ARGS__)


#define BUFFERMAX 1024
//读取和发送用一个数组标识，需要读偏移和写偏移
class Buffer
{
public:
    Buffer()
        : _read_pos(0),_write_pos(0),_buffer(BUFFERMAX)
    {}
    //总的容量
    int Capacity(){return _buffer.size();}
    //末尾空闲的大小
    int EndIdleSize(){return Capacity()-_write_pos;}
    //起始空闲的大小
    int StartIdleSize(){return _read_pos;}
    //获取起始地址
    char* Begin(){return &_buffer[0];}
    //获取读的起始地址
    char* ReadAddress(){return Begin()+_read_pos;}
    //获取可读数据的大小
    int EnableReadSize(){return _write_pos-_read_pos;}
    //获取可写的位置
    char* WriteAddress(){return Begin()+_write_pos;}
    //扩容
    void EnableWrite(int len)
    {
        //判断末尾是否够，够就直接写,不需要扩容
        if(EndIdleSize()>len){return;}
        //不够就看加上前面空闲的是否够，够就移动然后写
        if(EndIdleSize()+StartIdleSize()>len)
        {
            int ers=EnableReadSize();
            std::copy(ReadAddress(),ReadAddress()+ers,Begin());
            _read_pos=0;
            _write_pos=ers;
        }
        //不够就扩容
        else
        {
            int capacity=Capacity()+1024;
            _buffer.resize(capacity);
        }
    }
    //写入数据
    void Write(const void* date,int len)
    {
        //需要先判断是否可以写下，写不下需要扩容
        EnableWrite(len);
        char* d=(char*)date;
        std::copy(d,d+len-1,WriteAddress());
    }
    void WriteAndPush(const void* date,int len)
    {
        //需要先判断是否可以写下，写不下需要扩容
        EnableWrite(len);
        char* d=(char*)date;
        std::copy(d,d+len,WriteAddress());
        _write_pos+=len;
    }
    void WriteString(const std::string& date)
    {
        Write(date.c_str(),date.size());
    }
    void WriteStringAndPush(const std::string& date)
    {
        WriteAndPush(date.c_str(),date.size());
    }
    void WriteBuffer(Buffer& date)
    {
        Write(date.ReadAddress(),date.EnableReadSize());
    }
    void WriteStringAndPush(Buffer& date)
    {
        WriteAndPush(date.ReadAddress(),date.EnableReadSize());
    }

    //读取数据
    void Read(void* date,int len)
    {
        assert(len<=EnableReadSize());
        std::copy(ReadAddress(),ReadAddress()+len,(char*) date);
    }
    void ReadAndPop(void* date,int len)
    {
        assert(len<=EnableReadSize());
        std::copy(ReadAddress(),ReadAddress()+len,(char*) date);
        _read_pos+=len;
    }
    std::string ReadString(int len)
    {
        std::string date;
        date.resize(len);
        Read(&date[0],len);
        return date;
    }
    std::string ReadStringPop(int len)
    {
        std::string date;
        date.resize(len);
        ReadAndPop(&date[0],len);
        return date;
    }

    //清空缓冲区
    void Clear()
    {
        _read_pos=0;
        _write_pos=0;
    }

    std::vector<char> Buffers(){return _buffer;}
private:
    int _read_pos;               //读偏移
    int _write_pos;              //写偏移
    std::vector<char> _buffer;   //读取
};


class Socket
{
public:
    Socket(int socket=-1):_sockfd(socket)
    {}
    //创建套接字
    void Create()
    {
        _sockfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
        if(_sockfd<0)
        {
            ERR_LOG("create socket fail");
            abort();
        }
    }
    //绑定
    void Bind(const std::string &ip ,uint16_t port)
    {
        struct sockaddr_in client;
        memset((void*)&client,0,sizeof(client));
        client.sin_family=AF_INET;
        client.sin_addr.s_addr=inet_addr(ip.c_str());
        client.sin_port=htons(port);
        int n = bind(_sockfd,(const sockaddr*)&client,sizeof(client));
        if(n<0)
        {
            ERR_LOG("bind fail");
            abort();
        }
    }
    //监听
    void Listen()
    {
        int n =listen(_sockfd,0);
        if(n<0)
        {
            ERR_LOG("listen fail");
            abort();
        }
    }
    //连接服务器
    void Connect(const std::string &ip ,uint16_t port)
    {
        struct sockaddr_in server;
        memset((void*)&server,0,sizeof(server));
        server.sin_family=AF_INET;
        server.sin_addr.s_addr=inet_addr(ip.c_str());
        server.sin_port=htons(port);
        int n=connect(_sockfd,(const struct sockaddr*)&server,sizeof(server));
        if(n<0)
        {
            ERR_LOG("connect fail");
            abort();
        }

    }
    //接受连接
    int Accept()
    {
        int newfd=accept(_sockfd,nullptr,nullptr);
        if(newfd<0)
        {
            ERR_LOG("accept fail");
            abort();
        }
    }
    //发送信息
    ssize_t Send(void * buffer,int len,int flag=0)
    {   
        int n = send(_sockfd,buffer,len,flag);
        if(n<0)
        {
            if(errno==EWOULDBLOCK||errno==EINTR)
            {
                return 0;
            }
            ERR_LOG("send fail");
            return -1;
        }
        return n;
    }
    //接受信息
    ssize_t Recv(void * buffer,int len,int flag=0)
    {
        ssize_t n = recv(_sockfd,buffer,len,flag);
        if(n<0)
        {
            if(errno==EWOULDBLOCK||errno==EINTR)
            {
                return 0;
            }
            ERR_LOG("recv fail");
            return -1;
        }
        return n;
    }
    //关闭套接字
    void Close()
    {
        if(_sockfd!=-1)
        {
            close(_sockfd);
            _sockfd=-1;
        }

    }
    //设置套接字为非阻塞
    void SetNoBlock()
    {
        int flag = fcntl(_sockfd,F_GETFD,0);
        fcntl(_sockfd,flag|O_NONBLOCK);
    }
    //创建客户端连接
    void CreateClientConnect(const std::string& ip,uint32_t port)
    {
        Create();
        Connect(ip,port);
        INF_LOG("create client connect success,sockfd: %d",_sockfd);
    }
    //创建服务端连接
    void CreateServerConnect(uint32_t port,const std::string& ip="0.0.0.0")
    {
        Create();
        SetNoBlock();
        Bind(ip,port);
        Listen();     
        INF_LOG("create server connect success,sockfd: %d",_sockfd);
    }
    //获取描述符
    int Sockfd()
    {
        return _sockfd;
    }
private:
    int _sockfd;
};

class Poller;
//处理单个连接
class Channel
{
    using CallBack = std::function<void()>;
public:
    Channel(Poller* poller,int fd)
        :_poller(poller),_fd(fd),_events(0),_revents(0)
    {}
    //获取描述符
    int Fd(){return _fd;}
    //获取事件的状态
    uint32_t Event(){return _events;}
    //设置事件就绪
    void SetREvent(uint32_t event){_revents = event;}

    //设置各个回调  
    void SetReadCallBack(const CallBack& rb){_read_callback = rb;}
    void SetWriteCallBack(const CallBack& wb){_write_callback = wb;}
    void SetErrCallBack(const CallBack& eb){_err_callback = eb;}
    void SetCloseCallBack(const CallBack& cb){_close_callback = cb;}
    void SetEventsCallBack(const CallBack& eb){_events_callback = eb;}

    //是否监控了可读
    bool ReadAble(){return _events&EPOLLIN;}
    //是否监控了可写
    bool WriteAble(){return _events&EPOLLOUT;}
    //启动可读
    void EnableRead(){_events|=EPOLLIN;Update();}
    //启动可写
    void EnableWrite(){_events|=EPOLLOUT;Update();}
    //关闭可读
    void DisableRead(){_events&=~EPOLLIN;Update();}
    //关闭可写
    void DisableWrite(){_events&=~EPOLLOUT;Update();}
    //取消监控
    void DisableMonitor(){_events=0;Update();}
    //移除监控
    void Remove();
    //修改
    void Update();
    //对连接进行处理，去执行对应的回调
    void HandleEvents()
    {
        if((_revents&EPOLLIN)||(_revents&EPOLLRDHUP)||(_revents&EPOLLPRI)) 
        {
            if(_read_callback) 
                _read_callback();
        }
        //一次只会执行一个事件
        //例如写时，连接断开了，写就出错，还会往下进行，此时套接字已经关闭了，就会出错。
        if(_revents&EPOLLOUT)
        {
            if(_write_callback) 
                _write_callback();
        }
        else if(_revents&EPOLLERR)
        {
            if(_err_callback) 
                _err_callback();
        }
        else if(_revents&EPOLLHUP)
        {
            if(_close_callback)
                _close_callback();
        }
        //都会执行的回调
        if(_events_callback)
            _events_callback();
    }

private:
    int _fd;               //连接的描述符
    uint32_t _events;      //连接监控的事件
    uint32_t _revents;     //当前连接触发的事件
    Poller* _poller;

    CallBack _read_callback;   //可读事件的回调
    CallBack _write_callback;  //可写事件的回调
    CallBack _err_callback;    //错误事件的回调
    CallBack _close_callback;  //关闭事件的回调
    CallBack _events_callback; //任意事件的回调
};

//对所有连接进行监控
#define MAXEVENTSIZE 1024
class Poller
{
private:
    //对连接进行op操作
    void EpollCtl(Channel* channel,int op)
    {
        int fd=channel->Fd();
        struct epoll_event event;
        event.data.fd=fd;
        event.events=channel->Event();
        int n = epoll_ctl(_epollfd,op,fd,&event);
        if(n<0)
        {
            ERR_LOG("epoll ctl fail");
            abort();
        }
    }
public:
    Poller()
    {
        _epollfd=epoll_create(MAXEVENTSIZE);
        if(_epollfd<0)
        {
            ERR_LOG("epollfd create fail");
            abort();
        }
    }

    //增加监控
    void Add(Channel*channel)
    {
        _channels[channel->Fd()]=channel;
        EpollCtl(channel,EPOLL_CTL_ADD);
    }
    //修改
    void Update(Channel* channel)
    {
        EpollCtl(channel,EPOLL_CTL_MOD);
    }
    //移除
    void Erase(Channel* channel)
    {
        //先在hash中移除
        auto it=_channels.begin();
        while(it!=_channels.end())
        {
            if(it->first==channel->Fd())
            {
                _channels.erase(it);
                break;
            }
        }
        EpollCtl(channel,EPOLL_CTL_DEL);
    }
    //将就绪是将放到活跃队列里
    void Poll(std::vector<Channel*>* active) 
    {
        int n=epoll_wait(_epollfd,_events,MAXEVENTSIZE,-1);
        if(n<0)
        {
            ERR_LOG("epollfd wait fail");
            abort();
        }
        for(int i=0;i<n;i++)
        {
            int fd=_events->data.fd;
            _channels[fd]->SetREvent(_events->events);
            active->push_back(_channels[fd]);
        }
    }
private:
    int _epollfd;                                 //epoll的描述符
    std::unordered_map<int,Channel*> _channels;   //fd到Channel的映射
    struct epoll_event _events[MAXEVENTSIZE];
};

void Channel::Remove(){ _poller->Erase(this);}
void Channel::Update(){_poller->Update(this);}