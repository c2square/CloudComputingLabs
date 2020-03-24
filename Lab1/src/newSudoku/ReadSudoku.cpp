#include <bits/stdc++.h>
#include <pthread.h>
#include <sys/types.h>
#include "Sudoku.cpp"
using namespace std;

queue<Sudoku> problem;
queue<Sudoku> problem1;
queue<Sudoku> problem2;
queue<Sudoku> problem3;

typedef struct{
    int first;
    int last;
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

//下面是三个线程，从problem队列中分三部分提取到三个子队列中
void* divide1(void *args){
    Multhread* muk = (Multhread*) args;
    int first=muk->first;
    int last=muk->last;
    for(int i=first;i<last;i++)
    {
        problem1.push(problem.front());
        problem.pop();
    }
}

void* divide2(void *args){
    Multhread* muk = (Multhread*) args;
    int first=muk->first;
    int last=muk->last;
    for(int i=first;i<last;i++)
    {
        problem2.push(problem.front());
        problem.pop();
    }
}

void* divide3(void *args){
    Multhread* muk = (Multhread*) args;
    int first=muk->first;
    int last=muk->last;
    for(int i=first;i<last;i++)
    {
        problem3.push(problem.front());
    }
}

/*留出来的线程
void* divide(void *args){

}
*/

int main(int argc,char *argv[])
{
    string ss;
    while(cin>>ss)//输入文件路径
    {   
        int sum=readtx(ss);//problem队列中元素队个数
        //用三个线程来分发
        pthread_t th[4];
        Multhread thmuk[3];
        for(int i=0;i<3;i++)
        {
            int first=(int)(sum/3)*i;
            int last;
            if(i!=2) last=(int)(sum/3)*(i+1);
            else last=sum;
            thmuk[i].first=first;
            thmuk[i].last=last;
            if(i==0){
                if(pthread_create(&th[i],NULL,divide1,&thmuk[i])!=0)//线程创建
                {
                    perror("pthread_create failed");
                    exit(1);
                }
            }
            else if(i==1){
                if(pthread_create(&th[i],NULL,divide2,&thmuk[i])!=0)
                {
                    perror("pthread_create failed");
                    exit(1);
                }
            }
            else if(i==2){
                if(pthread_create(&th[i],NULL,divide3,&thmuk[i])!=0)
                {
                    perror("pthread_create failed");
                    exit(1);
                }
            }
        }

        /*
        留出来的线程
        if(pthread_create(&th[3],NULL,divide,NULL)!=0)
        {
            perror("pthread_create failed");
            exit(1);
        }*/
        
        for(int i=0;i<3;i++)
            pthread_join(th[i],NULL);
        
    }
}