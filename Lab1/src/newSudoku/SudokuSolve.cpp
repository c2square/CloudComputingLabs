#include <iostream>
#include <limits.h>
#include <vector>
#include <sstream>
using namespace std;
struct Node{
    Node *up, *down, *left, *right, *colRoot, *rowRoot;//���������ĸ�ָ���Լ�ָ�����ж����ָ��
    int Num;//�ж�������,��¼����
    int Size;//�ж�������,��¼����Ԫ����
    Node(int i = -1 ): Num(i),Size(0) {};//���캯��
};
class DLX{
public:
    DLX(vector<vector<int> > &matrix, int m, int n);
    ~DLX() { delete Head;};//�����е���д
    void init();
    void link(vector<vector<int> > &matrix);
    void cover(Node *cRoot);
    void recover(Node *cRoot);
    bool Search(int k = 0);
    vector<int> getResult() const { return result;}
    int getUpdates() const { return _updates;}
private:
    Node *Head;
    vector<int> result;//������������
    int _row, _col, _updates;//��¼������,���´���
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
    for (int ix = 0; ix < _col; ++ix)//��ͷλ��������,�����ж���
    {
        newNode = new Node;
        newNode->up = newNode;
        newNode->down = newNode;
        newNode->right = Head->right;
        newNode->left = Head;
        newNode->right->left = newNode;
        Head->right = newNode;
    }
    for (int ix = 0; ix < _row; ++ix)//��ͷλ�����²���,�����ж���
    {
        newNode = new Node(_row-ix);//ע�������_row-ix
        newNode->down = Head->down;
        newNode->up = Head;
        newNode->down->up = newNode;
        Head->down = newNode;
    }
}
void DLX::link(vector<vector<int> > &matrix)
{
    Node *current_row, *current_col, *newNode, *current;//��ǰ�ж���,��ǰ�ж���,�½ڵ�,��ǰ�ڵ�
    current_row = Head;
    for (int row = 0; row < _row; ++row)
    {
        current_row = current_row->down;
        current_col = Head;
        for (int col = 0; col < _col; ++col)
        {
            current_col = current_col->right;
 
            if (matrix[row][col] == 0)//������Ϊ0��λ�ò����ýڵ�
                continue;
 
            newNode = new Node;
 
            newNode->colRoot = current_col;
            newNode->rowRoot = current_row;//���õ�ǰ�ڵ��Ӧ�����ж���
 
            newNode->down = current_col;
            newNode->up = current_col->up;
            newNode->up->down = newNode;
            current_col->up = newNode;//���ӵ�ǰ�ڵ㵽��˫����β��
 
            if (current_row->Size == 0)//��˫������Ӧ�ð��ж����������
            {
                current_row->right = newNode;
                newNode->left = newNode;
                newNode->right = newNode;
                current_row->Size++;
            }
            current = current_row->right;//���õ�ǰ�ڵ�(���ж����ҵĽڵ�)
            newNode->left = current->left;
            newNode->right = current;
            newNode->left->right = newNode;
            current->left = newNode;//���ӵ�ǰ�ڵ㵽��˫����β��
 
            current_col->Size++;
        }
    }
}
void DLX::cover(Node *cRoot)//������
{
    ++_updates;
    cRoot->left->right = cRoot->right;
    cRoot->right->left = cRoot->left;//ɾ�����ж���
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
void DLX::recover(Node *cRoot)//�����㷨�ľ���!!
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
    if (Head->right == Head)//���,��ɹ��ҵ�һ���еļ���
        return true;
 
    Node *cRoot, *c;
    int minSize = INT_MAX;
    for(c = Head->right; c != Head; c = c->right)//������������ѡ���ж���
    {
        if (c->Size < minSize)
        {
            minSize = c->Size;
            cRoot = c;
            if (minSize == 1)
                break;
            if (minSize == 0)//��һ��Ϊ��,ʧ��
                return false;
        }
    }
    cover(cRoot);
 
    Node *current_row,*current;
    for (current_row = cRoot->down; current_row != cRoot; current_row = current_row->down)
    {
        result.push_back(current_row->rowRoot->Num);//�����м���result��
        for (current = current_row->right; current != current_row; current = current->right)
        {
            cover(current->colRoot);
        }
        if (Search(k+1))
            return true;
        for (current = current_row->left; current != current_row; current = current->left)
            recover(current->colRoot);
        result.pop_back();//���ָ��в�����Ҫ��,��ԭresult
    }
    recover(cRoot);
    return false;
}
vector<vector<int> > sudoku2matrix(string &problem)//������ת��Ϊ01����
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
vector<int> matrix2sudoku(vector<vector<int> > &matrix, vector<int> result)//��01������Ϊ����
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
//���涼�ǽ���ĺ��������ÿ� 
void solve_sudoku(string &problem)
{
    vector<vector<int> > matrix = sudoku2matrix(problem);
    DLX sudoku(matrix,matrix.size(),324);
    sudoku.Search();
    vector<int> solution = matrix2sudoku(matrix, sudoku.getResult());//���int���������������Ľ� 
    for(int ix=0;ix<81;++ix)
    {
		stringstream ss;//��int�������Ľ�ֱ�Ӹ�ֵ�������string��problem 
    	ss<<solution[ix];
    	ss>>problem[ix];
	}
}
int main()
{
    string problem;
    cin>>problem;
    solve_sudoku(problem);	
    cout<<problem;
}
/* 
027380010010006735000000029305692080000000000060174503640000000951800070080065340
000000520080400000030009000501000600200700000000300000600010000000000704000000030
800000000003600000070090200050007000000045700000100030001000068008500010090000400
*/ 
