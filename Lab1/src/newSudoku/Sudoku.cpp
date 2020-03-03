#include<string>
using namespace std;
/*
*������ʹ�÷�����
* ������������
* int a[81]={1,2,3,...,9};
* Sudoku one(a);
*/ 
class Sudoku
{
public:
    //��������������뵱ǰ��Ŀ��id���õ���һ����Ŀ��id��������֤id��Ψһ�� 
    static string getNextId(string curId);
    static bool first=true; 
    static string nowId;
    //sudoku����Ŀ��ans�Ǵ� 
    string id;
    int *sudoku;
    string ans;
    //problem ����������Ŀ 
    Sudoku(int problem[]);
};
//���캯�� 
Sudoku Sudoku::Sudoku(int problem[]){
	//��һ���ù��캯�� 
	if(first)
	{
		id="a";
		nowId="a";
		first=false;
	}
	//�ڶ��μ��Ժ� 
	else{
		id=Sudoku.getNextId(nowId);
		nowId=id;
	}
	
	sudoku=&problem[0];
	ans=""; 
}
//��ȡ��һ��id 
string sudoku::getNextId(string cur){
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
