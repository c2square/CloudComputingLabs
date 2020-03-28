#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "Sudoku.cpp"
#include <pthread.h>
using namespace std;
/*
*该类的使用方法,注意要看Sudoku的注释
* orderedList outBuffer;
* Sudoku s(problem);
* string answerOfs="123...89";
* //得到了数独s的答案就存到s的ans属性里，
* s.value=answerOfs;
* //然后调用outBuffer的add()方法,就可以自动排序并按id输出答案 
* outBuffer.add(s);
*/ 
class orderedList
{
private:
    //ofstream *output;
    pthread_mutex_t olock;
    list<Sudoku> buffer;
    string nowId = "0";
    //判断两个id的大小，如果a>b就返回true 
    bool aBigerb(string a,string b);
public:
    //添加一个数独到buffer（输出缓冲区）并按照id排序 ，并检查buffer第一个元素是否是需要输出 
    void add(Sudoku s);
    orderedList();
};
//
orderedList::orderedList(){
    if(freopen("output.txt","w",stdout)==NULL)
        cout<<"open file FAIL!"<<endl;
    /*output=new ofstream("/src/newSudoku/output.txt");
    if(!output->is_open()) cout<<"open file FAIL!"<<endl;*/
}
bool orderedList::aBigerb(string a,string b){
    if(a.size()==b.size()) return a>b;
    else return a.size()>b.size();
}

void orderedList::add(Sudoku s)
{
    pthread_mutex_lock(&olock);
    //cout<<"insert "<<s.id<<endl;
    //插入数独到列表中（排好序的插入） 
    if (buffer.empty())//如果为空放到最后 
        buffer.push_back(s);
    else//不然就找到第一个比s的id大的元素temp，插入到temp的前面 
    {
        list<Sudoku>::iterator it = buffer.begin();
        bool find = false;
        while (!find)
        {
        	//找到最后一个都没有比s大的，放到最后 
            if (it == buffer.end())
            {
                buffer.push_back(s);
                find = true;
            }
			//找到了比s大的，放到它的前面 
            else if (aBigerb(it->id,s.id))
            {
                buffer.insert(it, s);
                find = true;
            }
            //找下一个 
            it++;
        }
    }
    //循环检查缓冲区第一个元素是否需要输出
    while(true)
    {
    	//缓冲区空了 
        if (buffer.empty()) break;
        //缓冲区第一个需要输出，就输出并且更新下一个nowId
        if (buffer.front().id == nowId)
        {
            // //正式用
			 	cout << buffer.front().value<<" ";
			//测试用
            cout<<buffer.front().id<<endl;

            buffer.pop_front();
            nowId = Sudoku::getNextId(nowId);
        }
        //缓冲区第一个不需要输出，结束循环 
        else
            break;
    }
    pthread_mutex_unlock(&olock);
}
//下面是测试
//测试这个类的时候记得把Sukoku的main函数注释掉

// bool Sudoku::first=true;
// string Sudoku::nowId="";
// int main()
// {
//     orderedList outBuffer;
//     string problem="123456789";
//     Sudoku *a[650];
//     //生产大量数独
//     for(int i=0;i<650;i++){
//         a[i]=new Sudoku(problem);
//     }
//     int t1,t2;
//     Sudoku *temp;
//     //打乱顺序加入队列
//     for(int i=0;i<65;i++){
//         for(int k=0;k<5;k++){
//             t1=rand()%10; t2=rand()%10;
//             temp=a[i*10+t1];
//             a[i*10+t1]=a[i*10+t2];
//             a[i*10+t2]=temp;
//         }
//         for(int k=0;k<10;k++)
//             outBuffer.add(*a[i*10+k]);
//     }
//     //输出测试时输出id，正式应用输出ans，如果按照id顺序输出，则测试正确
//     return 0;
// }
