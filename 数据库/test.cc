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

    //mysql_set_character_set(my,"utf8");

    // std::string sql ="insert into user(name,age,telphone) values('李四',12,'124451115')";
    std::string sql="select * from user";
    int n =mysql_query(my,sql.c_str());
    if(n==0)std::cout<<sql<<" success: "<<n<<std::endl;
    else std::cout<<sql<<" err: "<<n<<std::endl;

    MYSQL_RES * res=mysql_store_result(my);
    if(res==nullptr)
    {
        std::cout<<"mysql_store_result err"<<std::endl;
        return 4;
    }
    my_ulonglong rows=mysql_num_rows(res);
    my_ulonglong fields=mysql_num_fields(res);

    MYSQL_FIELD *fields_arry=mysql_fetch_fields(res);
    for(int i=0;i<fields;i++)
    {
        std::cout<<fields_arry[i].name<<'\t';
    }
    std::cout<<std::endl;
    for(int i=0;i<rows;i++)
    {
        MYSQL_ROW row = mysql_fetch_row(res);
        for(int j=0;j<fields;j++)
        {
            std::cout<<row[j]<<'\t';
        }
        std::cout<<std::endl;
    }

    // std::string sql;
    // while(true)
    // {
    //     if(!std::getline(std::cin,sql)) break;
    //     if(sql=="quit") break;
    //     int n=mysql_query(my,sql.c_str());
    //     if(n==0)
    //     {
    //         std::cout<<sql<<" success: "<<n<<std::endl;
    //     }
    //     else
    //     {
    //         std::cout<<sql<<" err: "<<n<<std::endl;
    //     }
    // }

    mysql_free_result(res);
    mysql_close(my);
    
    return 0;
}