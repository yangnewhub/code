#pragma once
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
#include <condition_variable>
#include <unordered_map>
#include <sys/timerfd.h>
#include <assert.h>

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
#define  NWELIN  "\r\n"
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
    void ReadMove(int len)
    {
        _read_pos+=len;
    }
    void WriteMove(int len)
    {
        _write_pos+=len;
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
    void WriteBufferAndPush(Buffer& date)
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
    //获取一行
    char *FindCRLF() {
        char *res = (char*)memchr(ReadAddress(), '\n', EnableReadSize());
        return res;
    }
    /*通常获取一行数据，这种情况针对是*/
    std::string GetLine() {
        char *pos = FindCRLF();
        if (pos == NULL) {
            return "";
        }
        // +1是为了把换行字符也取出来。
        return ReadString(pos - ReadAddress() + 1);
    }
    std::string GetLineAndPop() {
        std::string str = GetLine();
        ReadMove(str.size());
        return str;
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
    bool Create()
    {
        _sockfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
        if(_sockfd<0)
        {
            ERR_LOG("create socket fail");
            return false;
        }

        int val = 1;
            setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, (void*)&val, sizeof(int)); 
        return true;


    }
    //绑定
    bool Bind(const std::string &ip ,uint16_t port)
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
            return false;
        }
        return true;
    }
    //监听
    bool Listen()
    {
        int n =listen(_sockfd,1024);
        if(n<0)
        {
            ERR_LOG("listen fail");
            return false;
        }
        return true;
    }
    //连接服务器
    bool Connect(const std::string &ip ,uint16_t port)
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
            return false;
        }
        return true;

    }
    //接受连接
    int Accept()
    {
        int newfd=accept(_sockfd,nullptr,nullptr);
        if(newfd<0)
        {
            ERR_LOG("accept fail");
            return -1;
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
    ssize_t NoBlackSend(void * buffer,int len)
    {
        return Send(buffer,len,MSG_DONTWAIT);
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
    ssize_t NoBlackRecv(void * buffer,int len)
    {
        return Recv(buffer,len,MSG_DONTWAIT);
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
    bool CreateClientConnect(const std::string& ip,uint32_t port)
    {
        if(Create()==false) return false;
        if(Connect(ip,port)==false) return false;
        INF_LOG("create client connect success,sockfd: %d",_sockfd);
        return true;
    }
    //创建服务端连接
    bool CreateServerConnect(uint32_t port,const std::string& ip="0.0.0.0")
    {
        if(Create()==false) return false ;
        SetNoBlock();
        if(Bind(ip,port)==false) return false;
        if(Listen()==false) return false;     
        INF_LOG("create server connect success,sockfd: %d",_sockfd);
        return true;
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
        auto it=_channels.find(channel->Fd());
        if(it!=_channels.end())
        {
            _channels.erase(it);
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
            auto it = _channels.find(fd);
            assert(it != _channels.end());
            
            it->second->SetREvent(_events[i].events);
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
        Pk p=it->second.lock();

        if(p)p->Cacnl();

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
            //DBG_LOG("yes");
            return cb();
            
        }
        //DBG_LOG("no");

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
    //移除
    void Erase(Channel* channel){ _poller.Erase(channel);}
    //修改
    void Update(Channel*channel){_poller.Update(channel);}
    //增加
    void TimerAdd(int id, const Factor& task,int timeout){return _timewheel.TimerAdd(id,task,timeout);}
    //刷新
    void TimerReferesh(int id){return _timewheel.TimerReferesh(id);}
    //取消
    void TimerCacnl(int id){return _timewheel.TimerCacnl(id);}
    //判断是否在
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

//eventloop和线程进行绑定，进行eventloop的创建和初始化
class LoopThread
{
private:
    void CreateAndStartEventLoop()
    {
        //先创建一个loop
        EventLoop loop;
        {
            //加锁进行赋值，然后条件条件变量
            std::unique_lock<std::mutex> lock(_mutex);
            _loop=&loop;
            _cond.notify_all();
        }
        //启动loop
        _loop->Start();
    }
public:
    LoopThread()
        :_thread(std::thread(&LoopThread::CreateAndStartEventLoop,this)),_loop(nullptr)
    {}
    EventLoop* GetLoop()
    {
        EventLoop * loop=nullptr;
        {
            std::unique_lock<std::mutex> lock(_mutex);
            _cond.wait(lock,[&](){return _loop!=nullptr;});
            loop=_loop;
        }
        return loop;
    }
private:
    EventLoop* _loop;      
    std::thread _thread;     //线程的
    std::mutex _mutex;             //锁
    std::condition_variable _cond; //条件变量
};

class LoopThreadPool
{
public:
    LoopThreadPool(EventLoop *acceptloop)
        :_thread_count(0),_pos_thread(0),_acceptloop(acceptloop)
    {}
    void SetThreadCount(int count)
    {
        _thread_count=count;
        if(count>0)
        {
            _threads.resize(count);
            _loops.resize(count);
            for(int i=0;i<count;i++)
            {
                _threads[i]=new LoopThread();
                _loops[i]=_threads[i]->GetLoop();
            }
        }
    }
    EventLoop* NextLoop()
    {
        if(_thread_count==0) return _acceptloop;
        _pos_thread=(_pos_thread+1)%_thread_count;
        return _loops[_pos_thread];
    }

private:
    int _thread_count;       //从属线程的数量
    std::vector<LoopThread*> _threads;//所有的从属线程
    std::vector<EventLoop*> _loops;//所有的evetnloop
    int _pos_thread;         //下一个线程
    EventLoop *_acceptloop; //接受连接的eventloop

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
    //DBG_LOG("执行刷新");
     _loop->RunInLoop(std::bind(&TimerWheel::TimerRefereshInLoop,this,id));
 }

 //删除
 void TimerWheel::TimerCacnl(int id)
 {
     _loop->RunInLoop(std::bind(&TimerWheel::TimerCacnlInLoop,this,id));
 }
 
class Any
{
private:
    class holder
    {
    public:
        virtual ~holder(){}
        virtual const std::type_info& type()=0;
        virtual holder* clone()=0;
    };
    template <class T>
    class placeholder:public holder
    {
    public:
        placeholder(const T& val){_val=val;}
        virtual const std::type_info& type(){return typeid(T);}
        virtual holder* clone(){return new placeholder(_val);}
    public:
        T _val;
    };
    holder * _val;
public:
    Any():_val(nullptr){}
    ~Any(){if(_val) delete _val;}
    template<class T>
    Any(const T& val){_val=new placeholder<T>(val);}
    Any(const Any& val){_val=(val._val?val._val->clone():nullptr);}

    Any& swap(Any other)
    {
        std::swap(_val,other._val);
        return *this;
    }
    template<class T>
    Any& operator=(const T& val)
    {
        swap(Any(val));
        return *this;
    }
    Any& operator=(const Any val)
    {
        swap(Any(val));
        return *this;
    }
    template <class T>
    T* get()
    {
        assert(typeid(T) == _val->type());
        return &((placeholder<T>*)_val)->_val;
    }
};
enum ConnectStatu
{
    DISCONNECTED, //关闭
    CONNECTING,   //半连接--刚连接上
    CONNECTED,    //连接
    DISCONNECTING //半关闭--准备关闭
};
class Connection;
using PtrConnection=std::shared_ptr<Connection>;
class Connection: public std::enable_shared_from_this<Connection>
{ 
    using ConnectCallBack=std::function<void(const PtrConnection&)>;
    using MessagCallBack=std::function<void(const PtrConnection&,Buffer*)>;
    using CloseCallBack=std::function<void(const PtrConnection&)>;
    using EventCallBack=std::function<void(const PtrConnection&)>;
private:
    //Channel的五个回调
    //读
    void HandlerRead()
    {
        //采用非阻塞读
        char buffer[65536]={0};
        int ret = _socket.NoBlackRecv(buffer,65535);
        //当ret<0时就是出错了，需要关闭连接
        if(ret<0)
        {
            
            //这个不是真正的关闭连接
            //需要将输入输出缓冲区中的内容处理掉然后关闭
            return ShutDownInLoop();
        }
        //将读到的数据写入读取缓冲区
        _in_buffer.WriteAndPush(buffer,ret);
        //处理用户的message回调
        if(_in_buffer.EnableReadSize()>0)
        {
            if (_message_callback)_message_callback(shared_from_this(),&_in_buffer);
        }
    }
    //写
    void HandlerWrite()
    {
        //非阻塞发送
        int ret=_socket.NoBlackSend(_out_buffer.ReadAddress(),_out_buffer.EnableReadSize());
        if(ret<0)
        {
            //发送错误，在这里就需要真正的关闭连接了
            //先处理掉发送缓冲区中的数据然后关闭
            if(_out_buffer.EnableReadSize()>0)
            {
                if (_message_callback)_message_callback(shared_from_this(),&_out_buffer);
            }
            return Release();
        }
        //发送完成，需要移动指针
        _out_buffer.ReadMove(ret);
        //当读完后需要将可读关闭，如果还处于半关闭状态，关闭
        if(_out_buffer.EnableReadSize()==0)
        {
            _channel.DisableWrite();
            if(_statu==DISCONNECTING)
            {
                return Release();
            }
        }
    }
    //关闭
    void HandlerClose()
    {
        //一旦出现问题，这个什么也不能干，处理下读到的数据，然后关闭
        if(_in_buffer.EnableReadSize()>0)
        {
            if (_message_callback)_message_callback(shared_from_this(),&_in_buffer);
        }
        return Release();
    }
    //错误
    void HandlerErr()
    {
        return HandlerClose();
    }
    //任意事件
    void HandlerEvent()
    {
        //有定时任务，就去刷新一下
        if(_istrue_timer) _loop->TimerReferesh(_conn_id);
        //有用户提供的就去执行一下
        if(_event_callback) _event_callback(shared_from_this());
    }
    //将数据从接收缓冲区移动到发送缓冲区
    void SendInLoop(Buffer & buff)
    {
        if(_statu==DISCONNECTED) return;
        //将内容写入发送缓冲区
        _out_buffer.WriteBufferAndPush(buff);
        //没有开启可写就开启
        if(_channel.WriteAble()==false) _channel.EnableWrite();
    }
    //开启定时任务
    void StartTimerTaskInLoop(int sec)
    {
        //状态开启
        _istrue_timer=true;
        //如果已经存在了，就刷新一下
        if(_loop->HasTimer(_conn_id)) _loop->TimerReferesh(_conn_id);
        //将任务加入到定时器中
        _loop->TimerAdd(_conn_id,std::bind(&Connection::Release,this),sec);
    }
    //取消定时任务
    void CancleTimerTaskInLoop()
    {
        //状态关闭
        _istrue_timer=false;
        //存在就取消
        if(_loop->HasTimer(_conn_id)) _loop->TimerCacnl(_conn_id);
    }
    //真正的关闭
    void ReleaseInLoop()
    {
        //DBG_LOG("开始关闭");
        //修改连接状态
        _statu=DISCONNECTED;
        //解除监控
        _loop->Erase(&_channel);
        //关闭文件描述符
        _socket.Close();
        //DBG_LOG("文件描述符关闭");
        
        //取消定时任务
        if(_istrue_timer) _loop->TimerCacnl(_conn_id);
       
        //执行用户给的回调
        if(_close_callback) _close_callback(shared_from_this());
        //DBG_LOG("close回调执行完毕");
        //执行组件内的回调
        if(_server_close_callback) _server_close_callback(shared_from_this());
        //DBG_LOG("回调执行完毕");


    }
    //没有直接关闭连接，需要将两个缓冲区中的内容处理
    void ShutDownInLoop()
    {  
        //设置为半连接
        _statu=DISCONNECTING;
        //读缓冲区
        if(_in_buffer.EnableReadSize()>0)
        {
            if (_message_callback) 
            {
                _message_callback(shared_from_this(),&_in_buffer);
            }
        }

        //写缓冲区
        if(_out_buffer.EnableReadSize()>0)
        {
            if (_channel.WriteAble() == false)
            {
                _channel.EnableWrite();
            }
        }
        else if(_out_buffer.EnableReadSize()==0)
        {   
            return Release();
        }

    }
   
    void ConnectInLoop()
    {
        assert(_statu==CONNECTING);
        //修改连接状态
        _statu=CONNECTED;
        //启动可读
        _channel.EnableRead();
        //执行用户的连接回调
        if(_connect_callback) _connect_callback(shared_from_this());
    }
    //修改协议
    void UpGardInLoop(const Any& context,
        const ConnectCallBack& connect_callback,
        const MessagCallBack& message_callback,
        const CloseCallBack& close_callback,
        const EventCallBack& event_callback
    )
    {
        _context=context;
        _connect_callback=connect_callback;
        _message_callback=message_callback;
        _close_callback=close_callback;
        _event_callback=event_callback;
    }
public:
        Connection(EventLoop* loop,int id,int sockfd)
        :_loop(loop),_conn_id(id),_sockfd(sockfd),_socket(sockfd),
        _channel(loop,sockfd),_istrue_timer(false),_statu(CONNECTING)
    {
        _channel.SetReadCallBack(std::bind(&Connection::HandlerRead,this));
        _channel.SetWriteCallBack(std::bind(&Connection::HandlerWrite,this));
        _channel.SetErrCallBack(std::bind(&Connection::HandlerErr,this));
        _channel.SetCloseCallBack(std::bind(&Connection::HandlerClose,this));
        _channel.SetEventsCallBack(std::bind(&Connection::HandlerEvent,this));

    }
    ~Connection()
    {
        //DBG_LOG("connectin break");
    }
    int Id(){return _conn_id;}
    int Fd(){return _sockfd;}
    bool IsConnect(){return _statu==CONNECTED;}
    void SetConText(const Any& context){_context=context;}
    Any* GetConText(){return &_context;}
    //设置几个回调
    // using ConnectCallBack=std::function<void(const PtrConnection&)>;
    // using MessagCallBack=std::function<void(const PtrConnection&,Buffer*)>;
    // using CloseCallBack=std::function<void(const PtrConnection&)>;
    // using EventCallBack=std::function<void(const PtrConnection&)>;
    void SetConnectCallBack(const ConnectCallBack& cb){_connect_callback=cb;}
    void SetMessagCallBack(const MessagCallBack& cb){_message_callback=cb;}
    void SetCloseCallBack(const CloseCallBack& cb){_close_callback=cb;}
    void SetEventCallBack(const EventCallBack& cb){_event_callback=cb;}
    void SetServeCloseCallBack(const CloseCallBack& cb ){_server_close_callback=cb;}
    //将数据从接收缓冲区移动到发送缓冲区
    void Send(char* date,int len)
    {
        Buffer buff;
        buff.WriteAndPush(date,len);
        _loop->RunInLoop(std::bind(&Connection::SendInLoop,this,buff));
    }
    //开启定时任务
    void StartTimerTask(int sec)
    {
        _loop->RunInLoop(std::bind(&Connection::StartTimerTaskInLoop,this,sec));
    }
    //取消定时任务
    void CancleTimerTask()
    {
        _loop->RunInLoop(std::bind(&Connection::CancleTimerTaskInLoop,this));

    }
    //真正的关闭
    void Release()
    {
        //_loop->RunInLoop(std::bind(&Connection::ReleaseInLoop,this));
        //注意这里必须加入任务队列，去执行，不然会出现野指针，如果RunInLoop在当前线程就会立即执行
        //但是任务队列中还可能会有任务，例如定时刷新的，此时已经释放，再执行就会出问题
        _loop->PushTask(std::bind(&Connection::ReleaseInLoop,this));
    }
    //关闭--不是真正的关闭
    void ShutDown()
    {
        _loop->RunInLoop(std::bind(&Connection::ShutDownInLoop,this));
    }
    //对象建立好后需要调用这个
    //需要一个连接去启动可读，主要是因为此时启动了定时任务，就可能出现问题，我们需要执行连接的回调
    void Connect()
    {
        _loop->RunInLoop(std::bind(&Connection::ConnectInLoop,this));
    }
    void UpGard(const Any& context,
        const ConnectCallBack& connect_callback,
        const MessagCallBack& message_callback,
        const CloseCallBack& close_callback,
        const EventCallBack& event_callback
    )
    {
        _loop->RunInLoop(std::bind(&Connection::UpGardInLoop,this,context,connect_callback
                        ,message_callback,close_callback,event_callback));

    }
private:
    int _conn_id;           //连接的id
    int _sockfd;            //连接的sockfd
    EventLoop* _loop;
    Socket _socket;
    Channel _channel;
    ConnectStatu _statu;    //连接的状态
    bool _istrue_timer;    //是否进行定时
    Buffer _in_buffer;      //输入缓冲区
    Buffer _out_buffer;     //输出缓冲区

    Any _context;
    //这四个需要组件使用者提供
    ConnectCallBack _connect_callback;
    MessagCallBack _message_callback;
    CloseCallBack _close_callback;
    EventCallBack _event_callback;

    CloseCallBack _server_close_callback;   
};


class Accepter
{
    using AccepteCallBack = std::function<void(int)>;
private:
    void HandlerRead()
    {

        //调用accept
        int nfd=_socket.Accept();
        if(nfd<0) return;
        //使用服务器给的回调
        if(_accept_callback)_accept_callback(nfd);

    }
    int CreateServer(int port)
    {
        bool ret=_socket.CreateServerConnect(port);
        assert(ret==true);
        return _socket.Sockfd();
    }
public:
    Accepter(EventLoop *loop,int port)
        :_socket(CreateServer(port)),_loop(loop),_channel(loop,_socket.Sockfd())
    {
        _channel.SetReadCallBack(std::bind(&Accepter::HandlerRead,this));
    }
    void SetAccepterCallBack(const AccepteCallBack& cb){_accept_callback=cb;}
    void Listen(){_channel.EnableRead();}
private:
    Socket _socket;
    Channel _channel;
    EventLoop* _loop;
    AccepteCallBack _accept_callback;
};

class TcpServer
{ 
    using Functor=std::function<void()>;
    using ConnectCallBack=std::function<void(const PtrConnection&)>;
    using MessagCallBack=std::function<void(const PtrConnection&,Buffer*)>;
    using CloseCallBack=std::function<void(const PtrConnection&)>;
    using EventCallBack=std::function<void(const PtrConnection&)>;
private:
    void NectConnect(int fd)
    {
        _id++;
        PtrConnection conn (new Connection(_pool.NextLoop(),_id,fd));
        _conns[_id]=conn;
        conn->SetConnectCallBack(_connect_callback);
        conn->SetMessagCallBack(_message_callback);
        conn->SetCloseCallBack(_close_callback);
        conn->SetEventCallBack(_event_callback);
        conn->SetServeCloseCallBack(std::bind(&TcpServer::RemoveConnect,this,std::placeholders::_1));
        if(_enable_timer_release) conn->StartTimerTask(_timeout);

        conn->Connect();
    }
    void AddTimerTaskInLoop(const Functor &task, int delay)
    {
        _id++;
        _acceptloop.TimerAdd(_id, task,delay);
    }
    void RemoveConnectInLoop(const PtrConnection& coon)
    {
        int id=coon->Id();
        auto it=_conns.find(id);
        if(it!=_conns.end())
        {
            _conns.erase(it);
        }
    }
    void RemoveConnect(const PtrConnection& coon)
    {
        _acceptloop.RunInLoop(std::bind(&TcpServer::RemoveConnectInLoop,this,coon));
    }
public: 
    TcpServer(int port):
        _port(port),
        _id(0),
        _accepter(&_acceptloop,port),
        _pool(&_acceptloop),
        _enable_timer_release(false)
        {
            _accepter.SetAccepterCallBack(std::bind(&TcpServer::NectConnect,this,std::placeholders::_1));
        }
    //设置线程池的数量
    void SetThreadPoolCount(int count){_pool.SetThreadCount(count);}
    //设置四个回调
    void SetConnectCallBack(const ConnectCallBack& cb){_connect_callback=cb;}
    void SetMessagCallBack(const MessagCallBack& cb){_message_callback=cb;}
    void SetCloseCallBack(const CloseCallBack& cb){_close_callback=cb;}
    void SetEventCallBack(const EventCallBack& cb){_event_callback=cb;}
    //开启定时任务
    void StartTimerTask(int timeout){_timeout=timeout;_enable_timer_release=true;}
    //添加定时任务
    void AddTimerTask(const Functor &task, int delay)
    {
        _acceptloop.RunInLoop(std::bind(&TcpServer::AddTimerTaskInLoop,this,task,delay));
    }   
    //服务器启动
    void Start(){_accepter.Listen();_acceptloop.Start();}

private:
    int _id; 
    int _port;                                          //端口
    EventLoop _acceptloop;                              //accepter的eventloop
    Accepter _accepter;     
    LoopThreadPool _pool;                               //线程池
    std::unordered_map<int, PtrConnection> _conns;      //每一个连接都有一个id
    int _timeout;                                       //超时时间
    bool _enable_timer_release;                         //是否开始超市任务

    //组件使用者提供的回调
   
    ConnectCallBack _connect_callback;
    MessagCallBack _message_callback;
    CloseCallBack _close_callback;
    EventCallBack _event_callback;
};