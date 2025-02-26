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
#include <thread>
#include <sys/eventfd.h>
#include <mutex>
#include <unordered_map>
#include <sys/timerfd.h>

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
        int n =listen(_sockfd,1024);
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
        return newfd;
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
class EventLoop;
//处理单个连接
class Channel
{
    using CallBack = std::function<void()>;
public:
    Channel(EventLoop* eventloop,int fd)
        :_eventloop(eventloop),_fd(fd),_events(0),_revents(0)
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
    EventLoop* _eventloop;

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
    bool HasEvents(Channel* channel)
    {
        auto it=_channels.find(channel->Fd());
        return it==_channels.end();
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
        if(HasEvents(channel)) 
        {
            Add(channel);
            //DBG_LOG("epoll add event success");
        }
        else
        {
            EpollCtl(channel,EPOLL_CTL_MOD);
            //DBG_LOG("epoll update event success");
        }
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
        //DBG_LOG("epoll remove event success");
    }
    //将就绪是将放到活跃队列里
    void Poll(std::vector<Channel*>* active) 
    {      
        int n=epoll_wait(_epollfd,_events,MAXEVENTSIZE,-1);
        //DBG_LOG("epoll_wait success, n: %d",n);

        if(n<0)
        {
            ERR_LOG("epollfd wait fail");
            abort();
        }
        for(int i=0;i<n;i++)
        {
            int fd=_events[i].data.fd;
            _channels[fd]->SetREvent(_events[i].events);
            active->push_back(_channels[fd]);
        }
    }
private:
    int _epollfd;                                  //epoll的描述符
    std::unordered_map<int,Channel*> _channels;   //fd到Channel的映射
    struct epoll_event _events[MAXEVENTSIZE];
};

 
class TimerTask
{
    using Factor = std::function<void()>;
public:
    TimerTask(int id,const Factor& task,int timeout)
        :_id(id),_task(task),_timeout(timeout),_start(true)
    {}
    ~TimerTask()
    {
        //如果时间到了，就执行定时任务
        if(_start) _task();
        _relase();
    }
    void Cacnl(){_start=false;}
    //返回超时时间
    int TimeOut(){return _timeout;}
    void SetRelase(const Factor& relase){_relase=relase;}
private:
    int _id;        //id
    Factor _task;   //定时的任务
    bool _start;    //是否运行
    int _timeout;   //超时时间
    Factor _relase;
};
//利用智能指针，
class TimerWheel
{
    using Factor = std::function<void()>;
    using Pk=std::shared_ptr<TimerTask>;
    using WeakPT=std::weak_ptr<TimerTask>;
private:
    static int CreateTimerFd()
    {
        int fd=timerfd_create(CLOCK_MONOTONIC,0);
        if(fd<0)
        {
            ERR_LOG("create timerfd fail");
            abort();
        }
        struct itimerspec newtime,oldtime;
        newtime.it_value.tv_nsec=0;   //第一次超时的毫秒 
        newtime.it_value.tv_sec=1;    //第一次超时的秒
        newtime.it_interval.tv_nsec=0;//第二次
        newtime.it_interval.tv_sec=1;
        int n = timerfd_settime(fd,0,&newtime,&oldtime);
        return fd;
        
    }
    void EraseInWheel(int id)
    {
        auto it=_wheel.find(id);
        if(it==_wheel.end()) return;
        _wheel.erase(it);
    }
    uint64_t ReadTimerFd()
    {
        uint64_t times;
        int n=read(_timerfd,&times,8);
        if(n<0)
        {
            ERR_LOG("read fail");
            abort();
        }
        return times;
    }
    void OnTime()
    {
        uint64_t times=ReadTimerFd();
        for(int i=0;i<times;i++)
        {
            Move();
        }
    }
    void TimerAddInLoop(int id, const Factor& task,int timeout)
    {
        //创建一个任务对象
        Pk pk=std::make_shared<TimerTask>(id,task,timeout);
        pk->SetRelase(std::bind(&TimerWheel::EraseInWheel,this,id));
        //超时位置
        int pos=(_pos+timeout)%_capcaity;
        _wheel[id]=WeakPT(pk);
        _timewheel[pos].push_back(pk);
    }
    void TimerRefereshInLoop(int id)
    {
        auto it = _wheel.find(id);
        if(it==_wheel.end()) return ;  //没找到，说明没这个任务，不需要刷新

        //刷新
        int timeout = (it->second.lock())->TimeOut();
        int pos=(_pos+timeout)%_capcaity;
        _timewheel[pos].push_back(it->second.lock());
    }
    void TimerCacnlInLoop(int id)
    {
        auto it = _wheel.find(id);
        if(it==_wheel.end()) return ;  //没找到，说明没这个任务，不需要取消
        it->second.lock()->Cacnl();
    }
public:
    TimerWheel(EventLoop* loop)
        :_pos(0),_capcaity(60),_timewheel(60),_loop(loop)
        ,_timerfd(CreateTimerFd()),_timerfd_channel(std::make_shared<Channel>(loop,_timerfd))
    {
        _timerfd_channel->SetReadCallBack(std::bind(&TimerWheel::OnTime,this));
        _timerfd_channel->EnableRead();
    }
    //刻度移动
    void Move()
    {
        _pos=(_pos+1)%_capcaity;
        _timewheel[_pos].clear();
        
    }
    bool HasTimer(int id)
    {
        auto it=_wheel.find(id);
        return it!=_wheel.end();
    }
    //增加
    void TimerAdd(int id, const Factor& task,int timeout);
    //刷新
    void TimerReferesh(int id);
   
    //删除
    void TimerCacnl(int id);
    int _pos;                                   //当前的时间可读
    int _capcaity;                              //最大的时间刻度
    std::vector<std::vector<Pk>> _timewheel;    //时间轮
    std::unordered_map<int,WeakPT> _wheel;      //id和weakptr对应
    EventLoop* _loop;

    int _timerfd;           //通过这个实现刻度移动，触发一次移动一次
    std::shared_ptr<Channel> _timerfd_channel;
};

//现在可以有一个任务队列
//Eventloop就是管理线程的模块，一个连接在多个线程中处理就会出现问题，加锁？特别浪费
//一个连接就有一个线程
class EventLoop
{
    using Factor=std::function<void()>;
private:
    static int CreateEventFd()
    {
        int fd= eventfd(0,EFD_CLOEXEC|EFD_NONBLOCK);
        if(fd<0)
        {
            ERR_LOG("eventfd create fail");
            abort();
        }
        return fd;
    }
    void ReadEventFd()
    {
        char buffer[1024]={0};
        int n=read(_eventfd,buffer,1023);
        if(n<0)
        {
            if(errno==EINTR||errno==EAGAIN)
            {
                return;
            }
            ERR_LOG("read fail");
            abort();
        }
    }
    void WriteEventFd()
    {
        int64_t val=1;
        int n=write(_eventfd,&val,8);
        if(n<0)
        {
            if(errno==EINTR||errno==EAGAIN)
            {
                return;
            }
            ERR_LOG("read fail");
            abort();
        }
    }
    //运行任务池中所有的任务
    void RunAllTask()
    {
        std::vector<Factor> task;
        {
            std::unique_lock<std::mutex> _lock(_mutex);
            swap(task,_task);
        }
        for(auto f:task)
        {
            f();
        }
    }
public:
    EventLoop()
        :_thread_id(std::this_thread::get_id()),
        _eventfd(CreateEventFd()),
        _eventfd_channel(std::make_shared<Channel>(this,_eventfd)),
        _timewheel(this)
    {
        _eventfd_channel->SetReadCallBack(std::bind(&EventLoop::ReadEventFd,this));
        //启动可读事件 
        _eventfd_channel->EnableRead();
    }
    //启动监控  执行就绪事件  执行任务池
    void Start()
    {
        while(1)
        {
            //启动监控
            std::vector<Channel*> active;
            _poller.Poll(&active);
            //执行就绪事件
            for(auto channel:active)
            {
                channel->HandleEvents();
            }
            //任务池x
            RunAllTask();
        }
    }   
    //是否在当前线程
    bool InThisThread()
    {
        return std::this_thread::get_id()==_thread_id;
    }
    //判断将要执行的任务是否处于当前线程中，如果是则执行，不是则压入队列。
    void RunInLoop(const Factor &cb) {
        if (InThisThread()) {
            return cb();
        }
        return PushTask(cb);
    }
    //将任务压入任务池
    void PushTask(const Factor& f)
    { 
        {
            std::unique_lock<std::mutex> _lock(_mutex);
            _task.push_back(f);
        }
        //由于可能epoll在等待不能就绪，所以需要唤醒
        WriteEventFd(); 
    }
    void Erase(Channel* channel){ _poller.Erase(channel);}
    void Update(Channel*channel){_poller.Update(channel);}
    void TimerAdd(int id, const Factor& task,int timeout){return _timewheel.TimerAdd(id,task,timeout);}
    void TimerReferesh(int id){return _timewheel.TimerReferesh(id);}
    void TimerCacnl(int id){return _timewheel.TimerCacnl(id);}
    bool HasTimer(int id){return _timewheel.HasTimer(id);}
private:
    std::thread::id _thread_id;                 //线程的id
    Poller _poller;                             //Poller
    int _eventfd;                               //eventfd
    std::shared_ptr<Channel> _eventfd_channel;
    std::vector<Factor> _task;                  //任务池
    std::mutex _mutex;                          //任务池加锁
    TimerWheel _timewheel;                      //时间轮
};


void Channel::Remove(){ _eventloop->Erase(this);}
void Channel::Update(){ _eventloop->Update(this);}


 //增加
 void TimerWheel::TimerAdd(int id, const Factor& task,int timeout)
 {
    _loop->RunInLoop(std::bind(&TimerWheel::TimerAddInLoop,this,id,task,timeout));
 }
 //刷新
 void TimerWheel::TimerReferesh(int id)
 {
     _loop->RunInLoop(std::bind(&TimerWheel::TimerRefereshInLoop,this,id));
 }

 //删除
 void TimerWheel::TimerCacnl(int id)
 {
     _loop->RunInLoop(std::bind(&TimerWheel::TimerCacnlInLoop,this,id));
 }