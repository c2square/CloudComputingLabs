<<<<<<< Updated upstream
#include <iostream>
#include <string>
#include "ThreadPool.h"
<<<<<<< Updated upstream
<<<<<<< Updated upstream
<<<<<<< Updated upstream
<<<<<<< Updated upstream
// #include "Sudoku.cpp"
#include "SudokuSolve.cpp"
=======
=======
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
=======
#include<bits/stdc++.h>
#include <Windows.h>
#include "Sudoku.cpp"
#include "SudokuSolve.cpp"
>>>>>>> Stashed changes
<<<<<<< Updated upstream
<<<<<<< Updated upstream
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
using namespace std;

bool Sudoku::first = true;
string Sudoku::nowId = "";
Sudoku * createShudokuList(){
    string problem = "027380010010006735000000029305692080000000000060174503640000000951800070080065340";
    Sudoku *sudokuList[10];
    for(int i=0;i<10;i++){
        sudokuList[i]=new Sudoku(problem);
        // printf("%d%s\n",i,sudokuList[i].value.c_str());
    }
    return * sudokuList;
}
int main()
{
    Sudoku *sudokuList=createShudokuList();
    for(int i=0;i<10;i++){
        printf("%d%s\n",i,sudokuList[i].value.c_str());
    }
    // std::mutex mtx;
    //     std::ThreadPool tp;
    //     std::vector<std::future<int>> v;
    //     std::vector<std::future<void>> v1;
    //     for (int i = 0; i <= 10; ++i)
    //     {
    //         auto ans = tp.add([](int answer) { return answer; }, i);
    //         v.push_back(std::move(ans));
    //     }
    //     for (int i = 0; i <= 5; ++i)
    //     {
    //         auto ans = tp.add([&mtx](const std::string& str1, const std::string& str2)
    //         {
    //             std::lock_guard<std::mutex> lg(mtx);
    //             std::cout << (str1 + str2) << std::endl;
    //             return;
    //         }, "hello ", "world");
    //         v1.push_back(std::move(ans));
    //     }
    //     for (size_t i = 0; i < v.size(); ++i)
    //     {
    //         std::lock_guard<std::mutex> lg(mtx);
    //         cout << v[i].get() << endl;
    //     }
    //     for (size_t i = 0; i < v1.size(); ++i)
    //     {
    //         v1[i].get();
    //     }

}