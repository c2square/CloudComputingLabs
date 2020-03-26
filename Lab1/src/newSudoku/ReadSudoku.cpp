#include <bits/stdc++.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include "Sudoku.cpp"
#define DNUM 1000000

using namespace std;

queue<Sudoku> problem;
queue<Sudoku> sproblem[DNUM];

sem_t empty,full,mutexl;

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
    sem_wait(&empty);
    sem_wait(&mutexl);
    for(int i=first;i<last;i++){
        sproblem[numbersqueue].push(problem.front());
        problem.pop();
    }
    sem_post(&mutexl);
    sem_post(&full);
}

//取
void *ftake(void *args){
    Multhread* muk = (Multhread*) args;
    int numbersqueue=muk->numbersqueue;
    for(int i=0;i<numbersqueue;i++){
        while(!sproblem[i].empty()){
            sem_wait(&full);
            sem_wait(&mutexl);
            //取出子队列中的元素，调用数独解决算法
            //sproblem[i].front();
            //sproblem[i].pop();
            //
            sem_post(&mutexl);
            sem_post(&empty);
        }
    }
}

//线程创建部分
void threadalloct(int sum,int threadnumber){
    pthread_t con;//取 线程
    pthread_t th[threadnumber];//放 线程
    Multhread thmuk[threadnumber];

    pthread_create(&con,NULL,ftake,NULL);
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

    pthread_join(con,NULL);
    for(int i=0;i<threadnumber;i++)
        pthread_join(th[i],NULL);
}

bool Sudoku::first=true;
string Sudoku::nowId="";

int main(int argc,char *argv[])
{
    string ss;
    while(cin>>ss)//输入文件路径
    {   
        int sum=readtx(ss);//problem队列中元素队个数
        int threadnumber=1;//线程数量
        if(argc>=2) threadnumber=atoi(argv[1]);
        if(threadnumber>DNUM) threadnumber=DNUM;

        sem_init(&empty,0,threadnumber);
        sem_init(&full,0,0);
        sem_init(&mutexl,0,1);

        threadalloct(sum,threadnumber);
        
    }
}