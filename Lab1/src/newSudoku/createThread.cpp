#include<bits/stdc++.h>
#include "../Sudoku/sudoku.h"
using namespace std;
/**Sudoku????
 * id??????
 * sudoku ???81??????
 * */
struct Sudoku{
    int id;
    int* sudoku;
};
/**questionList????
 * */
queue<Sudoku> questionList;
/**???????
 * ???????????2???*/
Sudoku fontAndPop(queue<Sudoku> &questionList){
    Sudoku result=questionList.front();
    questionList.pop();
    return result;
}
/**????81??????
 * ???????
 * */
int* createSudoku(){
    static int result[N];
    for (int i = 0; i < N; i++)
    {
        result[i]=rand()%10;
    }
    return result;
}
/**????sudoku??
 * ??????
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
/**?????????
 * ??????????????
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
    /**Windows?shell????c++????
     * 1. cd????????
     * 2. ?? g++ xxxx.cpp
     * 3. ?? ./a.exe
     */
    /** ?????????
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