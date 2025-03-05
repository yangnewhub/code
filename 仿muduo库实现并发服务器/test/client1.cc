//1     测试一个长连接    获取一行时有\r需要去掉
#include "../source/http/http.hpp"

// int main()
// {
//     Socket client;
//     client.CreateClientConnect("127.0.0.1",8888);
//     std::string req="GET hello HTTP/1.1\r\nConnection: keep-alive\r\nContent-Length: 0\r\n\r\n";
//     while(1)
//     {
//         client.Send((void*)req.c_str(),req.size());
//         char buf[1024]={0};
//         client.Recv(buf,1023);
//         DBG_LOG("%s",buf);
//         sleep(1);
//     }
// }


//2 测超时连接

// int main()
// {
//     Socket client;
//     client.CreateClientConnect("127.0.0.1",8888);
//     std::string req="GET hello HTTP/1.1\r\nConnection: keep-alive\r\nContent-Length: 0\r\n\r\n";
//     while(1)
//     {
//         client.Send((void*)req.c_str(),req.size());
//         char buf[1024]={0};
//         client.Recv(buf,1023);
//         DBG_LOG("%s",buf);
//         sleep(20);
//     }
// }

//3 给服务器发送1024字节，但是不足1024
//       1.发送一次，服务端得不到完整的请求，就不会进行业务处理，客户端得不到响应，超时关闭连接
//       2.发送多次小请求，服务器会将后边的请求当前面的正文进行处理，就会出现错误断开连接
//错误状态发送后需要重置context

// int main()
// {
//     Socket client;
//     client.CreateClientConnect("127.0.0.1",8888);
//     std::string req="GET hello HTTP/1.1\r\nConnection: keep-alive\r\nContent-Length: 100\r\n\r\nhahahahaha";
//     while(1)
//     {
//         //client.Send((void*)req.c_str(),req.size());
//         for(int i=0;i<11;i++)
//         {
//             client.Send((void*)req.c_str(),req.size());
//         }
//         char buf[1024]={0};
//         client.Recv(buf,1023);
//         DBG_LOG("%s",buf);
//         sleep(1);
//     }
// }



//4  业务处理超时
//1.在一次处理中消耗时间太长，导致其他的连接被连累，其他的可能会超时释放
//例如 有1 2 3 4 5就绪，处理1用30s就会导致2345没有刷新活跃度
//   1.如果2345是普通的就会被释放
//   2.如果2是定时器的模块的，超时触发，执行345就会野指针访问
//在本次事件处理，不能对连接进行释放，应该将释放操作放到任务池中，等到事件处理完后再执行任务池中的任务。
//根据超时次数执行移动

// int main()
// {

//     for(int i=0;i<10;i++)
//     {
//         pid_t pid=fork();
//         if(pid<0)
//         {
//             DBG_LOG("fail");
//             return -1;
//         }
//         else if(pid==0)
//         {
//             Socket client;
//             client.CreateClientConnect("127.0.0.1",8888);
//             std::string req="GET hello HTTP/1.1\r\nConnection: keep-alive\r\nContent-Length: 100\r\n\r\nhahahahaha";
//             while(1)
//             {
//                 client.Send((void*)req.c_str(),req.size());
//                 char buf[1024]={0};
//                 client.Recv(buf,1023);
//                 DBG_LOG("[%s]",buf);
//                 sleep(1);
//             }
//         }
//     }
   
// }

//5  一次多条请求

// int main()
// {
//     Socket client;
//     client.CreateClientConnect("127.0.0.1",8888);
//     std::string req="GET hello HTTP/1.1\r\nConnection: keep-alive\r\nContent-Length: 10\r\n\r\nhahahahaha";
//     req+="GET hello HTTP/1.1\r\nConnection: keep-alive\r\nContent-Length: 10\r\n\r\nhahahahaha";
//     req+="GET hello HTTP/1.1\r\nConnection: keep-alive\r\nContent-Length: 10\r\n\r\nhahahahaha";
//     while(1)
//     {
//         client.Send((void*)req.c_str(),req.size());
//         char buf[1024]={0};
//         client.Recv(buf,1023);
//         DBG_LOG("%s",buf);
//         sleep(1);
//     }
// }
