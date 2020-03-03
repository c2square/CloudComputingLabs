#include <list>
#include <string>
#include <iostream>
#include "sudoku.cpp"
using namespace std;

class orderedList
{
private:
    list<sudoku> buffer;
    string nowId = "a";
    
    bool aBigerb(string a,string b);
public:
    //add a sudoku to buffer,check the the first equel nowId
    void add(sudoku s);
};

bool orderedList::aBigerb(string a,string b){
    if(a.size()==b.size()) return a>b;
    else return a.size()>b.size();
}

void orderedList::add(sudoku s)
{
    //add sudoku s to the list;
    if (buffer.empty())
        buffer.push_back(s);
    else
    {
        list<sudoku>::iterator it = buffer.begin();
        bool find = false;
        while (!find)
        {
            if (it == buffer.end())
            {
                buffer.push_back(s);
                find = true;
            }
            else if (aBigerb(it->id,s.id))
            {
                buffer.insert(it, s);
                find = true;
            }
            it++;
        }
    }
    //check the list.front == nowId
    while(true)
    {
        if (buffer.empty()) break;
        if (buffer.front().id == nowId)
        {
            cout << buffer.front().ans << endl;
            buffer.pop_front();
            nowId = sudoku::getNextId(nowId);
        }
        else
            break;
    }
}