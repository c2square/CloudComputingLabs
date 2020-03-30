#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include <semaphore.h>
#include <bits/stdc++.h>
#include "SudokuSolve.cpp"
#include "orderedList.cpp"
#define MAX_CONCURRENT_INPUT_NUM 10000
#define MAX_URL_LENGTH 1024


bool Sudoku::first=true;
string Sudoku::nowId="";
orderedList outBuffer;

long int totalNumOfJobsDone=0;
pthread_mutex_t jobNumMutex=PTHREAD_MUTEX_INITIALIZER;

double time_diff(struct timeval x , struct timeval y)
{
  double x_ms , y_ms , diff;
  x_ms = (double)x.tv_sec*1000000 + (double)x.tv_usec;
  y_ms = (double)y.tv_sec*1000000 + (double)y.tv_usec;
  diff = (double)y_ms - (double)x_ms;
  if(diff<0)
  {
    fprintf(stderr, "ERROR! time_diff<0\n");
    exit(1);
  }
  return diff;
}


typedef struct {
  list<Sudoku> problems;
  long int inCount;
  long int outCount;
  pthread_mutex_t mutex;
  sem_t fullSlots; 
  sem_t emptySlots;
} JobQueue;

JobQueue inJobQueue;

void initJobQueue()
{
  inJobQueue.inCount=0;
  inJobQueue.outCount=0;
  pthread_mutex_init(&inJobQueue.mutex,NULL);
  sem_init(&inJobQueue.fullSlots, 0, 0); 
  sem_init(&inJobQueue.emptySlots, 0, MAX_CONCURRENT_INPUT_NUM);
}

Sudoku dequeueAJob()
{
  sem_wait(&inJobQueue.fullSlots);
  pthread_mutex_lock(&inJobQueue.mutex);

  Sudoku s("");
  if(!inJobQueue.problems.empty()){
    s=(Sudoku) inJobQueue.problems.front();
    inJobQueue.problems.pop_front();
  }
  inJobQueue.outCount++;
  pthread_mutex_unlock(&inJobQueue.mutex);
  sem_post(&inJobQueue.emptySlots); 
  return s;
}

void enqueueAJob(Sudoku job)
{
  sem_wait(&inJobQueue.emptySlots);
  pthread_mutex_lock(&inJobQueue.mutex);
  inJobQueue.problems.push_back(job);
  inJobQueue.inCount++;
  pthread_mutex_unlock(&inJobQueue.mutex);
  sem_post(&inJobQueue.fullSlots);
}

void* processJobsLongLiveThread(void* args) {
  
  while(1)
  {
    
    Sudoku oneSudoku=dequeueAJob();
    if(oneSudoku.value!=""){
      solve(&oneSudoku);
      outBuffer.add(oneSudoku);
    }
     
    //if(inJobQueue.outCount==inJobQueue.inCount) break;
    pthread_mutex_lock(&jobNumMutex);
    totalNumOfJobsDone++;
    pthread_mutex_unlock(&jobNumMutex);
  }
}

void readtx(string file){//读文件到队列中
    ifstream infile;
    infile.open(file.data());//将文件流对象与文件连接起来
    assert(infile.is_open());//若打开文件失败，则输出错误消息，并终止程序运行
    
    string s;
    int i=0;
    while(getline(infile,s)){//按行读取数据
        i++;
        Sudoku su(s);
        enqueueAJob(su);
    }
    //cout<<"读到了文件"<<endl;
    infile.close();//关闭文件
}

void waitForAllJobsDone(int finalJobID)
{
  //Lazily wait all the worker threads finish their wget jobs
  while(1)
  {
    usleep(10000);//Check per 10 ms
    pthread_mutex_lock(&jobNumMutex);
    if(totalNumOfJobsDone==finalJobID)//All jobs done
    {  
      pthread_mutex_unlock(&jobNumMutex);
      break;
    }
    pthread_mutex_unlock(&jobNumMutex);
  }
}

int main(int argc, char *argv[])
{
  int numOfWorkerThread=1;
  if(argc>=2)
    numOfWorkerThread=atoi(argv[1]);

  initJobQueue();

  // string ss;
  // cin>>ss;
  // readtx(ss);

  pthread_t th[numOfWorkerThread];
  int threadID[numOfWorkerThread];
  for(int i=0;i<numOfWorkerThread;i++)
  {
    threadID[i]=i;
    if(pthread_create(&th[i], NULL, processJobsLongLiveThread, &threadID[i])!=0)
    {
      perror("pthread_create failed");
      exit(1);
    }
  }
  // for(int i=0;i<numOfWorkerThread;i++)
  //       pthread_join(th[i],NULL);
 
  
  // while(cin>>ss){
  //   readtx(ss);
  // }
  char file[MAX_URL_LENGTH]={0}; 
  // while(scanf("%s",file)!=EOF){
  //   readtx(file);
  // }
  scanf("%s",file);
  struct timeval tvStart,tvEnd;
  gettimeofday(&tvStart,NULL);
  readtx(file);
  
  // sem_destroy(&inJobQueue.emptySlots);
  // sem_destroy(&inJobQueue.fullSlots);
  // pthread_mutex_destroy(&inJobQueue.mutex);
  waitForAllJobsDone(inJobQueue.inCount);
  gettimeofday(&tvEnd,NULL);
  printf("spend %.3lf\n",time_diff(tvStart,tvEnd)/1E6);
   printf("Finish all jobs! Get %ld URLs in total!\n",totalNumOfJobsDone);
   
  exit(0);
}