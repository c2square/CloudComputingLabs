#include<iostream>
#include<stdlib.h>
#include<cstring>
using namespace std;
int main(int argc,char **argv){
    char **t=argv;
    string opt,ip="localhost",port="8080",threadNum="2";
    //获取参数
    while(*t!=NULL){
        opt=*t;
        t++;
        if(opt=="--ip"){
            ip=*t;
            t++;
        }
        if(opt=="--port"){
            port=*t;
            t++;
        }
        if(opt=="--number-thread"){
            threadNum=*t;
            t++;
        }
    }
    //输出要运行的指令
    cout<<"java -jar Lab2.jar "+ip+" "+port+" "+threadNum<<endl;
    //打开运行jar包
    string command="java -jar Lab2.jar "+ip+" "+port+" "+threadNum;
    system(command.data());
    return 0;
}

