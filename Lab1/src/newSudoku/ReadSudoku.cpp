#include <bits/stdc++.h>
#include <pthread.h>
#include <sys/types.h>
#include "Sudoku.cpp"
#define DNUM 1000000

using namespace std;

queue<Sudoku> problem;
queue<Sudoku> sproblem[DNUM];

typedef struct{
    int first;
    int last;
    int numbersqueue;//分发的子队列数
}Multhread;

int readtx(string file){//读文件到队列中
    ifstream infile;
    infile.open(file.data());//将文件流对象与文件连接起来
    assert(infile.is_open());//若打开文件失败，则输出错误消息，并终止程序运行

    //分离文件名，并将其压入队列中
    string::size_type iPos = file.find_last_of('/')+1;
    string filename = file.substr(iPos,file.length()-iPos);
    problem.push({filename,1});

    string s;
    while(getline(infile,s)){//按行读取数据
        problem.push({s,0});//将每一行的数据压入队列中
    }
    infile.close();//关闭文件
    return problem.size();//返回队列中元素队个数
}

//创建线程队同时创建一个子队列，从problem队列中取一部分数
void* divide(void *args){
    Multhread* muk = (Multhread*) args;
    int first=muk->first;//截取problem队列中元素的开始位置
    int last=muk->last;//截取problem队列中元素的结束位置
    int numbersqueue=muk->numbersqueue;
    for(int i=first;i<last;i++){
        sproblem[numbersqueue].push(problem.front());
        problem.pop();
    }
}

int main(int argc,char *argv[])
{
    string ss;
    
    while(cin>>ss)//输入文件路径
    {   
        int sum=readtx(ss);//problem队列中元素队个数

        int threadnumber=1;//线程数量
        if(argc>=2) threadnumber=atoi(argv[1]);
        if(threadnumber>DNUM) threadnumber=DNUM;

        pthread_t th[threadnumber];
        Multhread thmuk[threadnumber];

        for(int i=0;i<threadnumber;i++){
            int first=(int)(sum/threadnumber)*i;
            int last;
            if(i!=2) last=(int)(sum/threadnumber)*(i+1);
            else last=sum;
            thmuk[i].first=first;
            thmuk[i].last=last;
            thmuk[i].numbersqueue=i;//子队列的下标

            if(pthread_create(&th[i],NULL,divide,&thmuk[i])!=0){
                perror("pthread_create failed");
                exit(1);
            }
        }

        for(int i=0;i<threadnumber;i++)
            pthread_join(th[i],NULL);
        
    }
}