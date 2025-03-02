#include <iostream>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <iostream>     
#include <fstream>
#include <regex>
#include "../server.hpp"

std::unordered_map<int, std::string> _statu_msg = {
    {100,  "Continue"},
    {101,  "Switching Protocol"},
    {102,  "Processing"},
    {103,  "Early Hints"},
    {200,  "OK"},
    {201,  "Created"},
    {202,  "Accepted"},
    {203,  "Non-Authoritative Information"},
    {204,  "No Content"},
    {205,  "Reset Content"},
    {206,  "Partial Content"},
    {207,  "Multi-Status"},
    {208,  "Already Reported"},
    {226,  "IM Used"},
    {300,  "Multiple Choice"},
    {301,  "Moved Permanently"},
    {302,  "Found"},
    {303,  "See Other"},
    {304,  "Not Modified"},
    {305,  "Use Proxy"},
    {306,  "unused"},
    {307,  "Temporary Redirect"},
    {308,  "Permanent Redirect"},
    {400,  "Bad Request"},
    {401,  "Unauthorized"},
    {402,  "Payment Required"},
    {403,  "Forbidden"},
    {404,  "Not Found"},
    {405,  "Method Not Allowed"},
    {406,  "Not Acceptable"},
    {407,  "Proxy Authentication Required"},
    {408,  "Request Timeout"},
    {409,  "Conflict"},
    {410,  "Gone"},
    {411,  "Length Required"},
    {412,  "Precondition Failed"},
    {413,  "Payload Too Large"},
    {414,  "URI Too Long"},
    {415,  "Unsupported Media Type"},
    {416,  "Range Not Satisfiable"},
    {417,  "Expectation Failed"},
    {418,  "I'm a teapot"},
    {421,  "Misdirected Request"},
    {422,  "Unprocessable Entity"},
    {423,  "Locked"},
    {424,  "Failed Dependency"},
    {425,  "Too Early"},
    {426,  "Upgrade Required"},
    {428,  "Precondition Required"},
    {429,  "Too Many Requests"},
    {431,  "Request Header Fields Too Large"},
    {451,  "Unavailable For Legal Reasons"},
    {501,  "Not Implemented"},
    {502,  "Bad Gateway"},
    {503,  "Service Unavailable"},
    {504,  "Gateway Timeout"},
    {505,  "HTTP Version Not Supported"},
    {506,  "Variant Also Negotiates"},
    {507,  "Insufficient Storage"},
    {508,  "Loop Detected"},
    {510,  "Not Extended"},
    {511,  "Network Authentication Required"}
};

std::unordered_map<std::string, std::string> _mime_msg = {
    {".aac",        "audio/aac"},
    {".abw",        "application/x-abiword"},
    {".arc",        "application/x-freearc"},
    {".avi",        "video/x-msvideo"},
    {".azw",        "application/vnd.amazon.ebook"},
    {".bin",        "application/octet-stream"},
    {".bmp",        "image/bmp"},
    {".bz",         "application/x-bzip"},
    {".bz2",        "application/x-bzip2"},
    {".csh",        "application/x-csh"},
    {".css",        "text/css"},
    {".csv",        "text/csv"},
    {".doc",        "application/msword"},
    {".docx",       "application/vnd.openxmlformats-officedocument.wordprocessingml.document"},
    {".eot",        "application/vnd.ms-fontobject"},
    {".epub",       "application/epub+zip"},
    {".gif",        "image/gif"},
    {".htm",        "text/html"},
    {".html",       "text/html"},
    {".ico",        "image/vnd.microsoft.icon"},
    {".ics",        "text/calendar"},
    {".jar",        "application/java-archive"},
    {".jpeg",       "image/jpeg"},
    {".jpg",        "image/jpeg"},
    {".js",         "text/javascript"},
    {".json",       "application/json"},
    {".jsonld",     "application/ld+json"},
    {".mid",        "audio/midi"},
    {".midi",       "audio/x-midi"},
    {".mjs",        "text/javascript"},
    {".mp3",        "audio/mpeg"},
    {".mpeg",       "video/mpeg"},
    {".mpkg",       "application/vnd.apple.installer+xml"},
    {".odp",        "application/vnd.oasis.opendocument.presentation"},
    {".ods",        "application/vnd.oasis.opendocument.spreadsheet"},
    {".odt",        "application/vnd.oasis.opendocument.text"},
    {".oga",        "audio/ogg"},
    {".ogv",        "video/ogg"},
    {".ogx",        "application/ogg"},
    {".otf",        "font/otf"},
    {".png",        "image/png"},
    {".pdf",        "application/pdf"},
    {".ppt",        "application/vnd.ms-powerpoint"},
    {".pptx",       "application/vnd.openxmlformats-officedocument.presentationml.presentation"},
    {".rar",        "application/x-rar-compressed"},
    {".rtf",        "application/rtf"},
    {".sh",         "application/x-sh"},
    {".svg",        "image/svg+xml"},
    {".swf",        "application/x-shockwave-flash"},
    {".tar",        "application/x-tar"},
    {".tif",        "image/tiff"},
    {".tiff",       "image/tiff"},
    {".ttf",        "font/ttf"},
    {".txt",        "text/plain"},
    {".vsd",        "application/vnd.visio"},
    {".wav",        "audio/wav"},
    {".weba",       "audio/webm"},
    {".webm",       "video/webm"},
    {".webp",       "image/webp"},
    {".woff",       "font/woff"},
    {".woff2",      "font/woff2"},
    {".xhtml",      "application/xhtml+xml"},
    {".xls",        "application/vnd.ms-excel"},
    {".xlsx",       "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"},
    {".xml",        "application/xml"},
    {".xul",        "application/vnd.mozilla.xul+xml"},
    {".zip",        "application/zip"},
    {".3gp",        "video/3gpp"},
    {".3g2",        "video/3gpp2"},
    {".7z",         "application/x-7z-compressed"}
};


class Util
{
public:
    //对字符串进行切割
    static void Spilt(const std::string& src,const std::string& sep,std::vector<std::string>* ret)
    {
        int index=0;
        int pos = src.find(sep,index);
        // abc.efg..aa.ccc
        while(pos!=std::string::npos)
        {
            if(pos!=index) ret->push_back(src.substr(index,pos-index));
            index=pos+sep.size();
            pos = src.find(sep,index);
        }
        if(index<src.size())
        {
            ret->push_back(src.substr(index));
        }

    }
    //读文件
    static bool ReadFile(const std::string& filename,char* buf)
    {
        std::ifstream is(filename.c_str(),std::ios::binary);
        if(!is.is_open())
        {
            std::cout<<"open file "<<filename<<"err"<<std::endl;
            return "";
        }
        //获取文件的大小
        struct stat st;
        int n = stat(filename.c_str(),&st);
        if(n<0)
        {
            std::cout<<"stat err"<<std::endl;
            return false;
        }
        int len=st.st_size;

        //进行读文件
        is.read(buf,len);
        if (!is) 
        {
            std::cout << "error: only " << is.gcount() << " could be read";
            return false;
        }
        is.close();
        return true;
    }
    //写文件
    static bool WriteFile(const std::string& filename,const std::string& buf)
    {
        std::ofstream os(filename.c_str(),std::ios::binary|std::ios::trunc);
        if(!os.is_open())
        {
            std::cout<<"open file "<<filename<<"err"<<std::endl;
            return false;
        }
        os.write(buf.c_str(),buf.size());
        if (os.good() == false) {
            ERR_LOG("WRITE %s FILE FAILED!", filename.c_str());
            os.close();    
            return false;
        }
        os.close();
        return true;

    }   
    //对Url进行编码
    //URL编码，避免URL中资源路径与查询字符串中的特殊字符与HTTP请求中特殊字符产生歧义
    //编码格式：将特殊字符的ascii值，转换为两个16进制字符，前缀%   C++ -> C%2B%2B
    //  不编码的特殊字符： RFC3986文档规定 . - _ ~ 字母，数字属于绝对不编码字符
    //RFC3986文档规定，编码格式 %HH 
    //W3C标准中规定，查询字符串中的空格，需要编码为+， 解码则是+转空格
    static std::string EnCode(const std::string& url,bool istrue)
    {
        std::string ret;
        for(auto c:url)
        {
            if(c=='.'||c=='-'||c=='_'||c=='~'||isalnum(c))
            {
                ret+=c;
                continue;
            }
            else if(c=='+'&&istrue)
            {
                ret+=' ';
                continue;
            }

            char ch[4]={0};
            snprintf(ch,4,"%%%02X",c);
            ret+=ch;
        }
        return ret;
    }

    //对url进行解码
    static char GetNum(char c)
    {
        if(c>='0'&&c<='9') return c-'0';
        else if(c>='a'&&c<='z') return (c-'a')+10;
        else if(c>='A'&&c<='Z') return (c-'A')+10;
        else return ' ';
    }

    static std::string DeCode(const std::string& url,bool istrue)
    {
        std::string ret;
        for(int i=0;i<url.size();i++)
        {
            if(url[i]=='.'||url[i]=='-'||url[i]=='_'||url[i]=='~'||isalnum(url[i]))
            {
                ret+=url[i];
                continue;
            }
            else if(url[i]==' '&&istrue)
            {
                ret+='+';
                continue;
            }
            //遇到%就将后面的转为数字16位转10位，然后后面一位<<4在+后面的第二位
            else if(url[i]=='%')
            {
                char num1 = GetNum(url[i+1]);
                char num2 = GetNum(url[i+2]);
                char num=(num1<<4)+num2;
                ret+=num;
                i+=2;
            }
           
        }
        return ret;
    }
    //获取状态码对应的信息
    static std::string StatuMessage(int code)
    {
        auto it = _statu_msg.find(code);
        if(it!=_statu_msg.end())
        {
            return it->second;
        }
        return "Not found";
    }
    //获取后缀对应的类型
    static std::string MiMeMessage(const std::string& filename)
    {
        //先获取后缀
        int pos= filename.find(".");
        if(pos==std::string::npos)
        {
            //没有就返回一个字节流
            return "application/octet-stream";
        }
        //有就初始化mime
        std::string mime = filename.substr(pos);
        auto it = _mime_msg.find(mime);
        if(it!=_mime_msg.end())
        {
            return it->second;
        }
        return "application/octet-stream";
    }
    //判断文件是不是目录
    static bool FileIsDir(const std::string& filename)
    {
        struct stat st;
        int ret = stat(filename.c_str(),&st);
        if (ret<0)
        {
            return false;
        }
        return  S_ISDIR(st.st_mode);
    }
    //判断文件是不是普通文件
    static bool FileIsRegular(const std::string& filename)
    {
        struct stat st;
        int ret = stat(filename.c_str(),&st);
        if (ret<0)
        {
            return false;
        }
        return  S_ISREG(st.st_mode);
    }
    //判断路径是否有效
    static bool PathIsVaile(const std::string& filename)
    {
        std::vector<std::string> path;
        Spilt(filename,"/",&path);
        int ret=0;
        for(auto p:path)
        {
            if(p=="..") 
            {
                ret--;
                if(ret<0) return false;
            }
            else if(p=="."){} 
            else ret++;
        }
        return true;
    }
};
class HttpRequest
{
public:
    HttpRequest():_version("HTTP/1.1"){}
    //查询头部是否存在
    bool HasHeander(const std::string& key)
    {
        auto it = _heads.find(key);
        if(it==_heads.end())
        {
            return false;
        }
        return true;
    }
    //插入头部
    void InsertHeader(const std::string& key,const std::string& val)
    {
        _heads[key]=val;
    }
    //查询头部的值
    std::string GetHeaderVal(const std::string& key)
    {
        auto it = _heads.find(key);
        if(it==_heads.end())
        {
            return "";
        }
        return it->second;
    }
    //查询字符串是否存在
    bool HasParams(const std::string& key)
    {
        auto it = _params.find(key);
        if(it==_params.end())
        {
            return false;
        }
        return true;
    }
    //插入字符串
    void InsertParams(const std::string& key,const std::string& val)
    {
        _params[key]=val;
    }
    //
    //查询字符串的值
    std::string GetParamsVal(const std::string& key)
    {
        auto it = _params.find(key);
        if(it==_params.end())
        {
            return "";
        }
        return it->second;
    }
    //获取正文长度
    long long GetBodySize()
    {
        if(HasHeander("Content-Length")) return std::stol(GetHeaderVal("Content-Length"));
        return 0;
    }
    //判断是不是短连接
    bool Close()
    {
        if(HasHeander("Connection")&&GetHeaderVal("Connection")=="keep-alive")  return false;
        return true;
    }
    //对所有数据进行清空
    void Clear()
    {
        _menoth.clear();
        _path.clear();
        _version.clear();
        _body.clear();
        std::smatch math;
        _matches.swap(math);
        _heads.clear();
        _params.clear();
    }
public:
    std::string _menoth;            //请求方法
    std::string _path;              //路径
    std::string _version;           //http版本
    std::string _body;              //正文
    std::smatch _matches;     //资源路径的正则提取数据
    std::unordered_map<std::string,std::string> _heads; //头部字段
    std::unordered_map<std::string,std::string> _params; //查询字符串
};
class HttpResponse 
{
public:
    HttpResponse():_redirect_flag(false),_statu(200){}
    HttpResponse(int statu):_redirect_flag(false),_statu(statu){}

    //查询头部是否存在
    bool HasHeander(const std::string& key)
    {
        auto it = _heads.find(key);
        if(it==_heads.end())
        {
            return false;
        }
        return true;
    }
    //插入头部
    void InsertHeader(const std::string& key,const std::string& val)
    {
        _heads[key]=val;
    }
    //查询头部的值
    std::string GetHeaderVal(const std::string& key)
    {
        auto it = _heads.find(key);
        if(it==_heads.end())
        {
            return "";
        }
        return it->second;
    }
        //判断是不是短连接
    bool Close()
    {
        if(HasHeander("Connection")&&GetHeaderVal("Connection")=="keep-alive")  return false;
        return true;
    }
    //对所有数据进行清空
    void Clear()
    {
        _statu=200;
        _body.clear();
        _redirect_flag=false;
        _redirect_url.clear();
        _heads.clear();
    }
    //添加正文
    void SetBody(const std::string&body,const std::string& type)
    {
        _body=body;
        InsertHeader("Content-Type", type);
    }
    //进行重定向
    void Redirect(const std::string& url,int statu=302)
    {
        _statu=statu;
        _redirect_flag=true;
        _redirect_url=url;
    }
public:
    int _statu;             //状态码
    std::string _body;      //正文
    bool _redirect_flag;    //是否进行重定向
    std::string _redirect_url;  //重定向的url
    std::unordered_map<std::string,std::string> _heads; //头部字段
};

enum RequestWait{
    RE_LINK,
    RE_LINE,
    RE_HEAD,
    RE_BODY,
    RE_ERR,
    RE_OVER
};
#define BUF_MAX_SIZE 8192
class HttpContext
{
private:
    //获取首行
    bool RecvLine(Buffer* buf)
    {
        if(_rewait!=RE_LINK) return false;
        //先获取首行的数据
        std::string line = buf->GetLineAndPop();
        if(line.size()==0)
        {
            //就是没有找到换行符，如果buf太长就认为出现错误
            if(buf->EnableReadSize()>BUF_MAX_SIZE)
            {
                _statu=414;//URI Too Long
                return false;
            }
            //说明请求没发完，等一等
            return true;
        }
        //读到了，需要进行解析
        if(PraseLine(line))
        {
            //解析成功，修改一下状态
            _rewait=RE_HEAD;
            return true;
        }
        //请求出现问题
        _statu  = 400;//Bad Request
        return false;
    }
    //解析首行
    bool PraseLine(const std::string& line)
    {
        // GET /a/b/c?password=123&user=aaa HTTP/1.1
        std::smatch match;
        std::regex c("(GET|POST|DELETE|HEAD) ([^?]*)(?:\\?(.*))? (HTTP/1\\.[01])(?:\r\n|\rn)?");
        int ret = std::regex_match(line,match,c);
        if(ret==false)
        {
            _statu  = 400;//Bad Request
            return false;
        } 

        _request._menoth=match[1];
        _request._path=Util::DeCode(match[2],false);
        _request._version=match[4];
        //字符串
        std::string s=match[3];
        //需要先分割 根据&进行分割
        std::vector<std::string> r;
        Util::Spilt(s,"&",&r);
        for(auto & x:r)
        {
            std::vector<std::string> v;
            Util::Spilt(x,"=",&v);
            _request.InsertParams(Util::DeCode(v[0],true),Util::DeCode(v[1],true));
        }
        return true;
    }
    //获取头部
    bool RecvHead(Buffer* buf)
    {
        if(_rewait!=RE_HEAD) return false;
        while(1)
        {
            //先获取首行的数据
            std::string line = buf->GetLineAndPop();
            if(line.size()==0)
            {
                //就是没有找到换行符，如果buf太长就认为出现错误
                if(buf->EnableReadSize()>BUF_MAX_SIZE)
                {
                    _rewait=RE_ERR;
                    _statu=414;//URI Too Long
                    return false;
                }
                //说明请求没发完，等一等
                return true;
            }

            //退出
            if(line=="\r\n"||line=="\n")
            {
                _rewait=RE_BODY;
                return true;
            }

            //找到了，进行解析
            if(!PraseHead(line))
            {
                _rewait=RE_ERR;
                _statu  = 400;//Bad Request
                return false;
            }
        }
    }
    //解析头部
    bool PraseHead(std::string& line)
    {
        int pos = line.find(": ");
        //没找到： 
        if(pos==std::string::npos)
        {
            _rewait=RE_ERR;
            _statu=400;
            return false;
        }
        //找到了
        std::string key = line.substr(0,pos);
        std::string val = line.substr(pos+2);
        _request.InsertHeader(key,val);
        return true;
    }
    //获取正文
    bool GetBody(Buffer* buf)
    {
        if(_rewait!=RE_BODY) return false;
        //先获取正文的长度
        int body_size = _request.GetBodySize();
        //为0直接true
        if(body_size==0) return true;
        //不为0看缓冲区中够不够
        //需要的长度
        int len =  body_size - _request._body.size();
        //够就读取需要的
        if(len <= buf->EnableReadSize())
        {
            _request._body.append(buf->ReadAddress(),len);
            buf->ReadMove(len);
            _rewait=RE_OVER;
            return true;
        }
        //不够就先读取有的
        _request._body.append(buf->ReadAddress(),buf->EnableReadSize());
        buf->ReadMove(buf->EnableReadSize());
        return true;
    }
public:
    HttpContext():_statu(200),_rewait(RE_LINK){}
    int Statu(){return _statu;}
    RequestWait NowWait(){return _rewait;}
    HttpRequest Request(){return _request;}
    void HttpParse(Buffer * buf)
    {
        RecvLine(buf);
        RecvHead(buf);
        GetBody(buf);
    }
private:
    int _statu;                 //响应码
    RequestWait _rewait;        //当前的状态
    HttpRequest _request;
};