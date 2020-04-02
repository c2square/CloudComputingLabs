#include <iostream>
#include <limits.h>
#include <vector>
#include <sstream>
#include "Sudoku.cpp"
#include<cmath>
#include<time.h>
using namespace std;
struct Node{
    Node *up, *down, *left, *right, *colRoot, *rowRoot;//上下左右四个指针以及指向行列对象的指针
    int Num;//行对象特有,记录行数
    int Size;//列对象特有,记录该列元素数
    Node(int i = -1 ): Num(i),Size(0) {};//构造函数
};
class DLX{
public:
    DLX(vector<vector<int> > &matrix, int m, int n);
    ~DLX() { delete Head;};//析构有点难写
    void init();
    void link(vector<vector<int> > &matrix);
    void cover(Node *cRoot);
    void recover(Node *cRoot);
    bool Search(int k = 0);
    vector<int> getResult() const { return result;}
    int getUpdates() const { return _updates;}
private:
    Node *Head;
    vector<int> result;//结果存放在这里
    int _row, _col, _updates;//记录行列数,更新次数
};
DLX::DLX(vector<vector<int> > &matrix, int m, int n)
    :_row(m),_col(n),_updates(0)
{
    Head = new Node;
    Head->up = Head;
    Head->down = Head;
    Head->right = Head;
    Head->left = Head;
    init();
    link(matrix);
}
void DLX::init()
{
    Node *newNode;
    for (int ix = 0; ix < _col; ++ix)//表头位置向后插入,构造列对象
    {
        newNode = new Node;
        newNode->up = newNode;
        newNode->down = newNode;
        newNode->right = Head->right;
        newNode->left = Head;
        newNode->right->left = newNode;
        Head->right = newNode;
    }
    for (int ix = 0; ix < _row; ++ix)//表头位置向下插入,构造行对象
    {
        newNode = new Node(_row-ix);//注意序号是_row-ix
        newNode->down = Head->down;
        newNode->up = Head;
        newNode->down->up = newNode;
        Head->down = newNode;
    }
}
void DLX::link(vector<vector<int> > &matrix)
{
    Node *current_row, *current_col, *newNode, *current;//当前行对象,当前列对象,新节点,当前节点
    current_row = Head;
    for (int row = 0; row < _row; ++row)
    {
        current_row = current_row->down;
        current_col = Head;
        for (int col = 0; col < _col; ++col)
        {
            current_col = current_col->right;
 
            if (matrix[row][col] == 0)//矩阵上为0的位置不设置节点
                continue;
 
            newNode = new Node;
 
            newNode->colRoot = current_col;
            newNode->rowRoot = current_row;//设置当前节点对应的行列对象
 
            newNode->down = current_col;
            newNode->up = current_col->up;
            newNode->up->down = newNode;
            current_col->up = newNode;//链接当前节点到列双向链尾端
 
            if (current_row->Size == 0)//行双向链不应该把行对象包含进来
            {
                current_row->right = newNode;
                newNode->left = newNode;
                newNode->right = newNode;
                current_row->Size++;
            }
            current = current_row->right;//设置当前节点(即行对象右的节点)
            newNode->left = current->left;
            newNode->right = current;
            newNode->left->right = newNode;
            current->left = newNode;//链接当前节点到行双向链尾端
 
            current_col->Size++;
        }
    }
}
void DLX::cover(Node *cRoot)//覆盖列
{
    ++_updates;
    cRoot->left->right = cRoot->right;
    cRoot->right->left = cRoot->left;//删除该列对象
    Node *i, *j;
    i = cRoot->down;
    while (i != cRoot)
    {
        j = i->right;
        while (j != i)
        {
            j->down->up = j->up;
            j->up->down = j->down;
            j->colRoot->Size--;
            j = j->right;
        }
        i = i->down;
    }
}
void DLX::recover(Node *cRoot)//整个算法的精髓!!
{
    Node *i, *j;
    i = cRoot->up;
    while (i != cRoot)
    {
        j = i->left;
        while (j != i)
        {
            j->colRoot->Size++;
            j->down->up = j;
            j->up->down = j;
            j = j->left;
        }
        i = i->up;
    }
    cRoot->right->left = cRoot;
    cRoot->left->right = cRoot;
}
bool DLX::Search(int k)
{
    if (Head->right == Head)//表空,则成功找到一组行的集合
        return true;
 
    Node *cRoot, *c;
    int minSize = INT_MAX;
    for(c = Head->right; c != Head; c = c->right)//根据启发条件选择列对象
    {
        if (c->Size < minSize)
        {
            minSize = c->Size;
            cRoot = c;
            if (minSize == 1)
                break;
            if (minSize == 0)//有一列为空,失败
                return false;
        }
    }
    cover(cRoot);
 
    Node *current_row,*current;
    for (current_row = cRoot->down; current_row != cRoot; current_row = current_row->down)
    {
        result.push_back(current_row->rowRoot->Num);//将该行加入result中
        for (current = current_row->right; current != current_row; current = current->right)
        {
            cover(current->colRoot);
        }
        if (Search(k+1))
            return true;
        for (current = current_row->left; current != current_row; current = current->left)
            recover(current->colRoot);
        result.pop_back();//发现该行不符合要求,还原result
    }
    recover(cRoot);
    return false;
}
vector<vector<int> > sudoku2matrix(string &problem)//将数独转换为01矩阵
{
    vector<vector<int> > matrix;
    for (int ix = 0; ix < 81; ++ix)
    {
        int val = problem[ix] - '0';
        vector<int> current_row(324,0);
        if (val != 0)
        {
            current_row[ix] = 1;
            current_row[81 + ix/9*9 + val -1] = 1;
            current_row[162 + ix%9*9 +val -1] = 1;
            current_row[243 + (ix/9/3*3+ix%9/3)*9 +val -1] = 1;
            matrix.push_back(current_row);
            continue;
        }
        for (int jx = 0; jx < 9; ++jx)
        {
            vector<int> current_row2(324,0);
            current_row2[ix] = 1;
            current_row2[81 + ix/9*9 + jx] = 1;
            current_row2[162 + ix%9*9 +jx] = 1;
            current_row2[243 + (ix/9/3*3+ix%9/3)*9 +jx] = 1;
            matrix.push_back(current_row2);
        }
    }
    return matrix;
}
vector<int> matrix2sudoku(vector<vector<int> > &matrix, vector<int> result)//将01矩阵翻译为数独
{
    vector<int> solution(81);
    for (int ix = 0; ix < 81; ++ix)
    {
        vector<int> current = matrix[result[ix]-1];
        int pos = 0, val = 0;
        for (int jx = 0; jx < 81; ++jx)
        {
            if (current[jx] == 1)
                break;
            ++pos;
        }
        for (int kx = 81; kx < 162; ++kx)
        {
            if (current[kx] == 1)
                break;
            ++val;
        }
        solution[pos] = val%9 + 1;
    }
    return solution;
}
//上面都是解题的函数，不用看 
void solve(Sudoku *n)
{
    vector<vector<int> > matrix = sudoku2matrix((*n).value);
    DLX sudoku(matrix,matrix.size(),324);
    sudoku.Search();
    vector<int> solution = matrix2sudoku(matrix, sudoku.getResult());//这个int型数组存的是数独的解 
	stringstream ss;//将int型数独的解直接赋值给输入的string型problem 
    for(int ix=0;ix<81;++ix)
    {
    	ss<<solution[ix];
    	ss>>(*n).value[ix];
	}
}
// bool Sudoku::first = true;
// string Sudoku::nowId = "";
// int main()
// {
// 	string value;
// 	string b;
// 	Sudoku s(value);
//     cin>>s.value;
//     b=s.value;
// 	Sudoku *n;
// 	n=&s;
// 	clock_t start,end;
//     start = clock();
//     for(int i=0;i<1000;i++)
//     {
// 	    s.value=b;
//     	solve(n);
// 	}
//     end = clock();
//     cout<<(*n).value<<endl;
//     cout<<(end-start)<<"ms"; 
// }
/* 
027380010010006735000000029305692080000000000060174503640000000951800070080065340
000000520080400000030009000501000600200700000000300000600010000000000704000000030
800000000003600000070090200050007000000045700000100030001000068008500010090000400
*/ 
