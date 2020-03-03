#include<string>
using namespace std;
class sudoku
{
    
    public:
    static string getNextId(string cur);
    
    string id;
    string ans;
    
};
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