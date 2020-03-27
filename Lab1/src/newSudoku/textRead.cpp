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

#define OUTPUT_FOLDER_NAME "testOutput"

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

typedef unsigned char BYTE;
int *threadFlags;

typedef struct {
  char **jobs;
  list<Sudoku> problems;
  int head;
  int tail;
  long int inCount;
  long int outCount;
  pthread_mutex_t mutex;
  sem_t fullSlots; 
  sem_t emptySlots;
  int queueCapacity;
} JobQueue;

JobQueue inJobQueue;

void initJobQueue()
{

  inJobQueue.head=0;
  inJobQueue.tail=0;
  inJobQueue.inCount=0;
  inJobQueue.outCount=0;
  inJobQueue.queueCapacity=MAX_CONCURRENT_INPUT_NUM; 

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
    // printf("before=%s\n",s.id.c_str());
    s=(Sudoku) inJobQueue.problems.front();
    inJobQueue.problems.pop_front();
    // printf("after1=%s\n",s.id.c_str());
  }
  // printf("dequeue id=%s size of queue=%lu\n",s.id.c_str(),inJobQueue.problems.size());
  inJobQueue.outCount++;
  pthread_mutex_unlock(&inJobQueue.mutex);
  sem_post(&inJobQueue.emptySlots); 
  return s;
}

void enqueueAJob(Sudoku job)
{
  sem_wait(&inJobQueue.emptySlots);
  pthread_mutex_lock(&inJobQueue.mutex);
  // printf("before0=%s\n",job.id.c_str());
  // printf("enqueue id=%s val=%s\n",job.id.c_str(),job.value.c_str());
  inJobQueue.problems.push_back(job);
  // printf("enqueue id=%s size of queue=%lu\n",job.id.c_str(),inJobQueue.problems.size());
  pthread_mutex_unlock(&inJobQueue.mutex);
  sem_post(&inJobQueue.fullSlots);
}


void* processJobsLongLiveThread(void* args) {
  int* id = (int*) args;
  // printf("id is %d\n",*id);
  long int numOfJobsIHaveDone=0;//Remember how many jobs I have done

  char command[MAX_URL_LENGTH+1000];
  struct timeval tvStart,tvEnd;
  while(1)
  {
    Sudoku oneSudoku=dequeueAJob();
    // printf("tid=%d id=%s size of queue=%lu\n",*id,oneSudoku.id.c_str(),inJobQueue.problems.size());
    // printf("after2=%s\n",oneSudoku.id.c_str());
    // gettimeofday(&tvStart,NULL);
    if(oneSudoku.value!=""){
      solve(&oneSudoku);
    //   printf("this id is %d the sudoku id is %s\n the val is %s\n",
    // *id,oneSudoku.id.c_str(),oneSudoku.value.c_str());
      outBuffer.add(oneSudoku);
    }
    
    
    // outBuffer.add(oneSudoku);

    numOfJobsIHaveDone++;

    pthread_mutex_lock(&jobNumMutex);
    totalNumOfJobsDone++;
    pthread_mutex_unlock(&jobNumMutex);
  }
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

void readtx(string file){//读文件到队列中
    ifstream infile;
    infile.open(file.data());//将文件流对象与文件连接起来
    assert(infile.is_open());//若打开文件失败，则输出错误消息，并终止程序运行

    //分离文件名，并将其压入队列中
    string::size_type iPos = file.find_last_of('/')+1;
    string filename = file.substr(iPos,file.length()-iPos);
    
    string s;
    int i=0;
    while(getline(infile,s)){//按行读取数据
        i++;
        Sudoku su(s);
        // printf("the sudoku is %s\n",su.value.c_str());
        enqueueAJob(su);
    }
    infile.close();//关闭文件
    // printf("the sum=%d",i);
}

int main(int argc, char *argv[])
{
  int numOfWorkerThread=1;
  if(argc>=2)
    numOfWorkerThread=atoi(argv[1]);

  initJobQueue();
  // initOutputFolder();

  printf("Start to wget urls ...\n");

  threadFlags=(int *)malloc(numOfWorkerThread*sizeof(threadFlags[0]));
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

  char file[MAX_URL_LENGTH]={0}; 
  while(scanf("%s",file)!=EOF)
  {
    string s=file;
    readtx(file);
    // while (!inJobQueue.problems.empty())
    // {
    //   Sudoku res=dequeueAJob();
    //   printf("after2=%s\n",res.id.c_str());
    //   solve(&res);
    //   // outBuffer.add(res);
    //   // printf("the sudoku id is %s\n the val is %s\n",
    //   // res.id.c_str(),res.value.c_str());
    // }
    
  }
  
  waitForAllJobsDone(inJobQueue.inCount);
  printf("Finish all jobs! Get %ld URLs in total!\n",totalNumOfJobsDone);
  printf("Use %d long-lived threads in total!\n",numOfWorkerThread);
  //In real project, do free the memory and destroy mutexes and semaphores
  exit(0);
}