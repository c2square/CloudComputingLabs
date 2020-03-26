#pragma once
#include <string>
#include <iostream>
using namespace std;
/*
*这个类的使用方法：
* 构造数独对象
* 特别注意！！！！！要在main 函数外面给Sudoku::first进行初始化,如下
* bool Sudoku::first=true;
* string nowId="";
* int main(){
*   string a="123.....89";
*   string filename="t1,txt"
*   Sudoku one(a,false);
*   //如果是文件就这样
*   Sudoku two(filename,true);
* }
*/

class Sudoku
{
public:
    //这个函数就是输入当前题目的id，得到下一个题目的id，用来保证id的唯一性
    static string getNextId(string cur);
    static bool first;
    static string nowId;
    static const int N = 81;
    //测试用
    static const int testN = 9;
    //sudoku是题目，ans是答案
    string id;
    string value;
    bool ifFile;
    //构造函数
    Sudoku(string problem,bool isfile);
};
string Sudoku::getNextId(string cur)
{
    int i = cur.size() - 1;
    while (true)
    {
        if (i == -1)
        {
            cur = 'a' + cur;
            break;
        }
        if (cur[i] == 'z')
        {
            cur[i] = 'a';
            i--;
        }
        else
        {
            cur[i]++;
            break;
        }
    }
    return cur;
}
//problem 就是数独题目,isfile是用来区分这是题目还是文件
Sudoku::Sudoku(string problem,bool isfile)
{
    //第一调用构造函数
    if (first)
    {
        id = "a";
        nowId = "a";
        first = false;
    }
    //第二次及以后
    else
    {
        id = getNextId(nowId);
        nowId = id;
    }

    value = problem;
    ifFile = isfile;
}
//测试这个类就把下面的注释解除。测试orderedList类就保持注释
/*bool Sudoku::first = true;
string Sudoku::nowId = "";
int main()
{
    string problem = "123456789";
    for (int i = 0; i < 27; i++)
    {
        for (int i = 0; i < 27; i++)
        {
            Sudoku a(problem,false);
            cout << a.id << ' ';
        }
        cout << endl;
    }
    //test id in order is right
    return 0;
}*/