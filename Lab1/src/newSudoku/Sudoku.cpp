#include<string>
using namespace std;
/*
*这个类的使用方法：
* 构造数独对象
* 特别注意！！！！！要在main 函数外面给Sudoku::first进行初始化,如下
* bool Sudoku::first=true;
* int main(){
*   int a[81]={1,2,3,...,9};
*   Sudoku one(a);
* }
*/ 

class Sudoku
{
public:
    //这个函数就是输入当前题目的id，得到下一个题目的id，用来保证id的唯一性 
    static string getNextId(string curId);
    static bool first; 
    static string nowId;
	static const int N=81;
    //sudoku是题目，ans是答案 
    string id;
    int *sudoku;
    int *ans;
    //problem 就是数独题目 
    Sudoku(int problem[]);
};
//构造函数 
Sudoku::Sudoku(int problem[]){
	//第一调用构造函数 
	if(first)
	{
		id="a";
		nowId="a";
		first=false;
	}
	//第二次及以后 
	else{
		id=Sudoku::getNextId(nowId);
		nowId=id;
	}
	
	sudoku=&problem[0];
}
//获取下一个id 
string Sudoku::getNextId(string cur){
    int i=cur.size()-1;
    while(true)
    {
        if(i==-1)
        {
            cur='a'+cur;
            break;
        } 
        if(cur[i]=='z')
        {
            cur[i]='a';
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
