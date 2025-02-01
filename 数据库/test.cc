#include <iostream>
#include <string>
#include <mysql/mysql.h>

const std::string host="localhost";
const std::string user="yang";
const std::string password="yang123456";
const std::string db ="conn";
const unsigned int port =3306;
int main()
{
    //std::cout<<"mysql client version: "<<mysql_get_client_info()<<std::endl;
    MYSQL* my= mysql_init(nullptr);
    if(my==nullptr)
    {
        std::cerr<<"init MySQL error"<<std::endl;
        return 1;
    }

    if((mysql_real_connect(my,host.c_str(),user.c_str(),password.c_str(),db.c_str(),port,nullptr,0))==nullptr)
    {
        std::cout<<"connect err"<<std::endl;
        return 2;
    }
    std::cout<<"connect success"<<std::endl; 


    mysql_close(my);
    
    return 0;
}