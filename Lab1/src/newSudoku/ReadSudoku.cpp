#include <stdio.h>  
#include <malloc.h>
#include <stdlib.h>
#include <string.h> 
  
typedef struct Sudoku  
{  
    int data;  
    struct Sudoku* next;  
}Sudoku;    
 
//初始化   
Sudoku* NewQueue()  
{  
    Sudoku* LQ = (Sudoku*)malloc(sizeof(Sudoku));  
    LQ->data = 0;  
    LQ->next = NULL;   
} 

//进队   
void Push(Sudoku* LQ, int data)  
{  
    int i;  
    Sudoku* temp = NewQueue();  
    temp->data = data;  
    while(LQ->next != NULL)  
    {  
        LQ = LQ->next;  
    }  
    LQ->next = temp;  
}

//出队  
Ele Pop(Sudoku * LQ)  
{  
    Sudoku * temp;  
    int data;  
      
    if(LQ->next == NULL)  
    {  
        return data;  
    }  
    temp = LQ->next;  
    data = temp->data;  
    LQ->next = temp->next;  
    free(temp);  
    return data;      
}

//是否为空   
int IsEmpty(Sudoku * LQ)  
{  
    if(LQ->next == NULL)  
    {  
        return 1;  
    }  
    else  
    {  
        return 0;  
    }  
}

//遍历  
void Traserval(Sudoku* LQ)  
{  
    printf("\n");  
    while(LQ->next != NULL)   
    {  
        printf("%d\n", LQ->next->data);  
        LQ = LQ->next;  
    }  
}
 
int main()  
{  
    Sudoku *LQ = NewQueue();
	int size=10240;
	char* s=(char*)malloc(size);
	while(gets(s)!=NULL){//读取文件位置信息 
		FILE *filp;
		filp=fopen(s,"r");//打开文件为只读形式 
	    if(filp){//判断打开文件是否成功 
	    	char c;
			int linelength=0;
			while(!feof(filp))//读取文件的行数 
			{
				c=fgetc(filp);
				if(c=='\n') linelength++;
			}
			for(int i=0;i<=linelength;i++)
			{
				int num[linelength+1];
				fscanf(filp,"%d",&num[i]);
				Push(LQ,num[i]);//将每一行的数据压入队列中 
			} 
		}
	} 
    return 0;  
}  