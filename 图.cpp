#include <bits/stdc++.h>
#define SIZE 7
using namespace std;

typedef enum poi_status
{undiscover,discover,visited}poi_status;
typedef enum edge_status
{nottree,tree}edge_status;

template<class T>class point
{
public:
    T data;
    poi_status status;
    unsigned int d_time,f_time;
    point(T a){data=a;status=undiscover;}
    point(){status=undiscover;}
};
template<class Tp,class Te>class edge
{
public:
    point<Tp> *poi_0,*poi_1;
    Te data;
    edge_status status;
    edge(point<Tp> *p0,point<Tp> *p1,Te a)
    {poi_0=p0;poi_1=p1;data=a;status=nottree;}
    edge(point<Tp> *p0,point<Tp> *p1)
    {poi_0=p0;poi_1=p1;status=nottree;}
};

template<class Tp,class Te>class graph
{
    typedef point<Tp> P;
    typedef edge<Tp,Te> E;
    typedef void(*FUNC)(Tp);
private:
    vector<P>poi_set;
    vector<vector<E*> > matrix;
    void dfs_son(unsigned int n,FUNC visit,unsigned int &Clock)
    {
        unsigned int i;
        poi_set[n].status=discover;
        poi_set[n].d_time=++Clock;
        visit(poi_set[n].data);
        for(i=0;i<poi_set.size();i++)if(matrix[n][i])
        {
            if(poi_set[i].status==undiscover)
                dfs_son(i,visit,Clock);
        }
        poi_set[n].status=visited;
        poi_set[n].f_time=++Clock;
    }
public:
    void add_point(Tp a)
    {
        unsigned int _size=poi_set.size();
        poi_set.push_back(P(a));
        matrix.push_back(vector<E*>());
        for(unsigned int i=0;i<_size;i++)
            matrix[i].push_back((E*)NULL);
        for(unsigned int i=0;i<_size+1;i++)
            matrix[_size].push_back((E*)NULL);
    }
    void del_point(unsigned int n)
    {
        unsigned int _size=poi_set.size();
        poi_set.erase(poi_set.begin()+n);
        for(unsigned int i=0;i<_size;i++)
            matrix[i].erase(matrix[i].begin()+n);
        matrix.erase(matrix.begin()+n);
    }
    void add_edge(unsigned int p0,unsigned int p1,Te a)
    {
        if(!matrix[p0][p1])
            matrix[p0][p1]=new E(&poi_set[p0],&poi_set[p1],a);
        else matrix[p0][p1]->data=a;
    }
    bool is_edge(unsigned int p0,unsigned int p1)
    {
        return matrix[p0][p1]!=NULL;
    }
    Te del_edge(unsigned int p0,unsigned int p1)
    {
        Te t=matrix[p0][p1]->data;
        delete matrix[p0][p1];
        matrix[p0][p1]=NULL;
        return t;
    }
    void bfs(unsigned int n,FUNC visit)
    {
        unsigned int f,i;
        for(i=0;i<poi_set.size();i++)
            poi_set[i].status=undiscover;
        queue<unsigned int> Q;
        poi_set[n].status=discover;Q.push(n);
        while(!Q.empty())
        {
            f=Q.front();visit(poi_set[f].data);Q.pop();
            for(i=0;i<poi_set.size();i++)if(matrix[f][i])
            {
                if(poi_set[i].status==undiscover)
                {Q.push(i);poi_set[i].status=discover;
                matrix[f][i]->status=tree;}
                else matrix[f][i]->status=nottree;
            }
            poi_set[f].status=visited;
        }
    }
    void dfs(unsigned int n,FUNC visit,unsigned int &Clock)
    {
        unsigned int i;
        for(i=0;i<poi_set.size();i++)
            poi_set[i].status=undiscover;
        dfs_son(n,visit,Clock);
        for(i=0,cout<<'\n';i<poi_set.size();i++)
            cout<<poi_set[i].data<<' '<<poi_set[i].d_time<<' '<<poi_set[i].f_time<<endl;
    }
};
void PRINT(int n){cout<<n<<' ';}
int main()
{
    unsigned int Clock=0;
    graph<int,int> G;
    for(unsigned int i=0;i<SIZE;i++)
        G.add_point(i+1);
    G.add_edge(0,1,1);
    G.add_edge(0,2,2);
    G.add_edge(1,3,3);
    G.add_edge(1,4,4);
    G.add_edge(2,5,5);
    G.add_edge(2,6,6);
    G.bfs(0,PRINT);cout<<'\n';
    G.dfs(0,PRINT,Clock);
    return 0;
}
