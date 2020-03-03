#include <list>
#include <string>
#include <iostream>
#include "Sudoku.cpp"
using namespace std;
/*
*�����ʹ�÷���
* orderedList outBuffer;
* Sudoku s(problem);
* string answerOfs="????...???"
* //�õ�������s�Ĵ𰸾ʹ浽s��ans�����
* s.ans=answerOfs;
* //Ȼ�����outBuffer��add()����,�Ϳ����Զ����򲢰�id����� 
* outBuffer.add(s);
*/ 
class orderedList
{
private:
    list<sudoku> buffer;
    string nowId = "a";
    //�ж�����id�Ĵ�С�����a>b�ͷ���true 
    bool aBigerb(string a,string b);
public:
    //���һ��������buffer�������������������id���� �������buffer��һ��Ԫ���Ƿ�����Ҫ��� 
    void add(Sudoku s);
};
// 
bool orderedList::aBigerb(string a,string b){
    if(a.size()==b.size()) return a>b;
    else return a.size()>b.size();
}

void orderedList::add(Sudoku s)
{
    //�����������б��У��ź���Ĳ��룩 
    if (buffer.empty())//���Ϊ�շŵ���� 
        buffer.push_back(s);
    else//��Ȼ���ҵ���һ����s��id���Ԫ��temp�����뵽temp��ǰ�� 
    {
        list<Sudoku>::iterator it = buffer.begin();
        bool find = false;
        while (!find)
        {
        	//�ҵ����һ����û�б�s��ģ��ŵ���� 
            if (it == buffer.end())
            {
                buffer.push_back(s);
                find = true;
            }
			//�ҵ��˱�s��ģ��ŵ�����ǰ�� 
            else if (aBigerb(it->id,s.id))
            {
                buffer.insert(it, s);
                find = true;
            }
            //����һ�� 
            it++;
        }
    }
    //ѭ����黺������һ��Ԫ���Ƿ���Ҫ���
    while(true)
    {
    	//���������� 
        if (buffer.empty()) break;
        //��������һ����Ҫ�������������Ҹ�����һ��nowId
        if (buffer.front().id == nowId)
        {
            cout << buffer.front().ans << endl;
            buffer.pop_front();
            nowId = Sudoku.getNextId(nowId);
        }
        //��������һ������Ҫ���������ѭ�� 
        else
            break;
    }
}
