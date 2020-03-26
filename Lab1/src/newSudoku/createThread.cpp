#include <bits/stdc++.h>
#include "Sudoku.cpp"
// #include "ReadSudoku.cpp"
#include "orderedList.cpp"
#include <unistd.h>
#include "SudokuSolve.cpp"
using namespace std;
int numberOfThread=10;
mutex mtx;
orderedList outBuffer;

queue<Sudoku> problems;
bool Sudoku::first = true;
string Sudoku::nowId = "";

void  createShudokuList(){
    string pro = "027380010010006735000000029305692080000000000060174503640000000951800070080065340";
    // string problem="";
    for(int i=0;i<10;i++){
        // problem+=i+'0';
        problems.push({pro,0}) ;
        // printf("%d %s\n",i,sudokuList[i].value.c_str());
    }
}
void *text(void *arg){
    queue<Sudoku> i=* (queue<Sudoku> *) arg;
    printf("This tid is %lu\n", pthread_self());
    printf("This sudoku is OX%p\n",&i);
    while (problems.size())
    {
        solve(&problems.front());
        outBuffer.add(problems.front());
        printf("This is number of %s\n",problems.front().value.c_str());
        problems.pop();
    }
}

int main()
{
    createShudokuList();
    for(int i=0;i<numberOfThread;i++){
        printf("\nThe time is %d\n",i);
        pthread_t th1;
        if(pthread_create(&th1, NULL, text, (void *)&problems)!=0)
        {
            perror("pthread_create failed");
            exit(1);
        }
        sleep(1);
    }
}