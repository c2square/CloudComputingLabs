#include<bits/stdc++.h>
#include "../Sudoku/sudoku.h"
using namespace std;
/**Sudoku数据结构
 * id序号
 * sudoku 题目
 * */
struct Sudoku{
    int id;
    int* sudoku;
};
/**questionList问题列表
 * */
queue<Sudoku> questionList;
/**弹出第一个元素
 * 合成一个函数，使其原子化*/
Sudoku fontAndPop(queue<Sudoku> &questionList){
    Sudoku result=questionList.front();
    questionList.pop();
    return result;
}
/**创建随机数独
 * 样例，不用管
 * */
int* createSudoku(){
    static int result[N];
    for (int i = 0; i < N; i++)
    {
        result[i]=rand()%10;
    }
    return result;
}
/**打印数独内容
 * 格式
 * {id,xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx}
 * */
void printfSudoku(Sudoku result){
    printf("{%d,",result.id);
        for (int j = 0; j < N; j++)
        {
            printf("%d",*(result.sudoku+j));
        }
        printf("}\n");
}
/**解数独函数
 * 随便写的，用于测试
 * */
void sudokuSolve(queue<Sudoku> questionList){
    Sudoku question;
    while (!questionList.empty())
    {
        question=fontAndPop(questionList);
        for (int i = 0; i < N; i++){
            if (*(question.sudoku+i)==0)
            {
                *(question.sudoku+i)=-1;
            }
        }
        printfSudoku(question);
    }
}
int main(){
    /**Windows下shell编译运行c++文件的方法：
     * 1. cd 到当前目录
     * 2. 键入 g++ xxxx.cpp
     * 3. 键入 ./a.exe
     */
    /** 生成问题列表

     * */
    for (int i = 0; i < 100; i++){
        Sudoku sudokuTmp={
            i,
            createSudoku()
        }; 
        questionList.push(sudokuTmp);
    }
    sudokuSolve(questionList);
    return 0;
    
}