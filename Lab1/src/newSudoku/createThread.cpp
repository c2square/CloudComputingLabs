#include<bits/stdc++.h>
#include<windows.h>
#include "sudoku.cpp"
#include "SudokuSolve.cpp"
using namespace std;
static int  N =81;
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
    static int result[81];
    srand(time(0));
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
    printf("{%s,",result.id.c_str());
        for (int j = 0; j < N; j++)
        {
            // printf("%d",*(result.sudoku+j));
        }
        printf("}\n");
}
/**解数独函数
 * 随便写的，用于测试
 * */

int main(){
    /**Windows下shell编译运行c++文件的方法：
     * 1. cd 到当前目录
     * 2. 键入 g++ xxxx.cpp
     * 3. 键入 ./a.exe
     */
    /** 生成问题列表
     * */
    for (int i = 0; i < 10; i++){
        // Sudoku sudokuTmp={
        //     to_string(i),
        //     createSudoku()
        // }; 
        // questionList.push(sudokuTmp);
        
    }
    // sudokuSolve(questionList);
    return 0;
    
}