#include<bits/stdc++.h>
using namespace std;
int a[81];
struct DLX
{
    int n,m,si;
    int u[2966],d[2966],r[2966],l[2966],row[2966],col[2966];
    int h[739],s[334];
    int ans[334];
    void init(int nn,int mm)
    {
        n=nn,m=mm;
        for(int i=0;i<=m;i++)
        {
            s[i]=0;
            u[i]=d[i]=i;
            l[i]=i-1,r[i]=i+1;
        }
        l[0]=m,r[m]=0;
        si=m;
        for(int i=1;i<=n;i++)
            h[i]=-1;
    }

    void add(int rr,int cc)
    {
        si++;
        row[si]=rr;
        col[si]=cc;
        s[cc]++;

        d[si]=d[cc];
        u[d[cc]]=si;
        u[si]=cc;
        d[cc]=si;

        if(h[rr]<0) h[rr]=l[si]=r[si]=si;
        else
        {
            r[si]=r[h[rr]];
            l[r[h[rr]]]=si;
            l[si]=h[rr];
            r[h[rr]]=si;
        }
    }

    void _remove(int c)
    {
        l[r[c]]=l[c],r[l[c]]=r[c];
        for(int i=d[c];i!=c;i=d[i])
        {
            for(int j=r[i];j!=i;j=r[j])
            {
                u[d[j]]=u[j];
                d[u[j]]=d[j];
                s[col[j]]--;
            }
        }
    }

    void _resume(int c)
    {
        for(int i=u[c];i!=c;i=u[i])
        {
            for(int j=l[i];j!=i;j=l[j])
            {
                d[u[j]]=j;
                u[d[j]]=j;
                s[col[j]]++;
            }
        }
        l[r[c]]=r[l[c]]=c;
    }

    bool dance(int st)
    {
        if(r[0]==0)
        {
            for(int i=0;i<st;i++) a[(ans[i]-1)/9]=(ans[i]-1)%9+1;
        }

        int c=r[0];
        for(int i=r[0];i!=0;i=r[i])
            if(s[i]<s[c]) c=i;

        _remove(c);
        for(int i=d[c];i!=c;i=d[i])
        {
            ans[st]=row[i];
            for(int j=r[i];j!=i;j=r[j]) _remove(col[j]);
            if(dance(st+1)) return true;
            for(int j=l[i];j!=i;j=l[j]) _resume(col[j]);
        }
        _resume(c);
        return false;
    }
};


void get_p(int &rr,int &c1,int &c2,int &c3,int &c4,int i,int j,int k)
{
    rr=(i*9+j)*9+k;
    c1=i*9+j+1;
    c2=9*9+i*9+k;
    c3=9*9*2+j*9+k;
    c4=9*9*3+((i/3)*3+(j/3))*9+k;
}


DLX sp;

int su(int *a,int *b)
{
        for(int i=0;i<81;i++)
        {
        	a[i]=b[i];
		}
        sp.init(9*9*9,9*9*4);
        int rr,c1,c2,c3,c4;
        for(int i=0;i<9;i++)
        {
            for(int j=0;j<9;j++)
            {
                for(int k=1;k<=9;k++)
                {
                    if(a[i*9+j]==0||a[i*9+j]==k)
                    {
                        get_p(rr,c1,c2,c3,c4,i,j,k);
                        sp.add(rr,c1);
                        sp.add(rr,c2);
                        sp.add(rr,c3);
                        sp.add(rr,c4);
                    }
                }
            }
        }
        sp.dance(0);
        for(int i=0;i<81;i++)
        {
        	b[i]=a[i];
		}
        return *b;

}

int main()
{     
    int problem[81];  
        for(int i=0;i<9;i++)
        {
            for(int j=0;j<9;j++)
               cin>>problem[i*9+j];
        }
	   su(a,problem);     
	   for(int i=0;i<9;i++)
        {
        for(int j=0;j<9;j++)
        {
        if(j!=0) putchar(' ');
            cout<<problem[i*9+j];
       }
            putchar('\n');
        } 
}
/* 
4 0 0 0 7 0 1 0 0
0 0 1 9 0 4 6 0 5
0 0 0 0 0 1 0 0 0
0 0 0 7 0 0 0 0 2
0 0 2 0 3 0 0 0 0
8 4 7 0 0 6 0 0 0
0 1 4 0 0 0 8 0 6
0 2 0 0 0 0 3 0 0
6 0 0 0 9 0 0 0 0
*/
