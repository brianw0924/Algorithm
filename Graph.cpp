#include <iostream>
#include <string.h>
#include <vector>
#include <queue>
#include <set>
#include <list>
#include <map>
#include <stack>
#include <algorithm>
#include <math.h>
#include <ctype.h>
#include <utility>
using namespace std;
# define infty 2147483647
struct vertex{
    int number, distance, discovery, finish, pi;
    char color;
    vertex(){}
};

class Graph{
    public : 
    int V;
    int t; // for DFS discovery time
    list<int> T_sort;
    vector<vertex> Vertice;
    vector<list<int>> Adjlist;
    
    Graph(int n){
        this->V = n;
        this->t = 0;
        Vertice.resize(n+1);
        Adjlist.resize(n+1); // the vertex no. could be 1~n, not including 0
    }
    void addEdge_undirected(int u, int v);
    void addEdge_directed(int u, int v);
    void BFS(int start);
    void DFS_visit(int u);
    void DFS(int start);
    void SCC();
    void Topological_Sort(int start);
    int Diameter();
    void printGraph();
};

void Graph::addEdge_undirected(int u, int v){
    Adjlist[u].push_back(v);
    Adjlist[v].push_back(u);
}

void Graph::addEdge_directed(int u, int v){
    Adjlist[u].push_back(v);
}

void Graph::BFS(int start){
    for(int i=1;i<=V;i++){
        Vertice[i].number = i;
        Vertice[i].distance = infty; // in BFS, d is distance from starting vertex
        Vertice[i].pi = -1;
        Vertice[i].color = 'W';
    }
    Vertice[start].distance = 0;
    Vertice[start].color = 'G';
    queue<int> Q;
    Q.push(start);
    while(!Q.empty()){
        int u = Q.front();
        Q.pop();
        for(list<int>::iterator it = Adjlist[u].begin();it!=Adjlist[u].end();it++){
            if(Vertice[*it].color == 'W'){
                Vertice[*it].distance = Vertice[u].distance + 1;
                Vertice[*it].pi = u;
                Vertice[*it].color = 'G';
                Q.push(*it);
            }
        }
        Vertice[u].color = 'B';
    }
}
void Graph::DFS_visit(int u){
    t++;
    Vertice[u].discovery = t;
    Vertice[u].color = 'G';
    for(list<int>::iterator it = Adjlist[u].begin();it!=Adjlist[u].end();it++){
        if(Vertice[*it].color == 'W'){
            Vertice[*it].pi = u;
            DFS_visit(*it);
        }
    }
    t++;
    Vertice[u].color = 'B';
    T_sort.push_back(u); // for Topological sort & SCC
    Vertice[u].finish = t;
}
void Graph::DFS(int start){
    for(int i=1;i<=V;i++){
        Vertice[i].number = i;
        // in DFS, d is dicovery time;
        Vertice[i].pi = -1;
        Vertice[i].color = 'W';
    }
    for(int i=1;i<=V;i++){
        if(Vertice[i].color == 'W')
            DFS_visit(i);
    }
    this->t = 0;
}

void Graph::SCC(){
    Graph RG(V); // Reverse Graph
    for(int i=1;i<=V;i++){
        RG.Vertice[i].number = i;
        // in DFS, d is dicovery time;
        RG.Vertice[i].pi = -1;
        RG.Vertice[i].color = 'W';
    }
    for(int i=1;i<=V;i++){ // Reverse the Edges
        for(list<int>::iterator it=Adjlist[i].begin();it!=Adjlist[i].end();it++){
            RG.Adjlist[*it].push_back(i);
        }
    }

    this->DFS(1); // In order to find the finish time in decreasing order
    int c=0; // count how many SCC

    /* DFS in Reverse Graph by selecting vertex in decreasing finish time */
    for(list<int>::reverse_iterator it=this->T_sort.rbegin();it!=this->T_sort.rend();it++){
        if(RG.Vertice[*it].color == 'W'){
            c++;
            RG.DFS_visit(*it); // every time we DFS (if valid, i.e color=='W') , we got a SCC set
            printf("Strongly Connected Component set [%d] : ",c);
            for(list<int>::iterator it=RG.T_sort.begin(), end=RG.T_sort.end();it!=end;it=RG.T_sort.erase(it)){
                printf("%d ",*it);
            }
            printf("\n");
        }
    }
}


void Graph::Topological_Sort(int start){
    DFS(start);
    list<int>::iterator re = T_sort.begin(), rb = --T_sort.end();
    for(list<int>::iterator it = rb;it!=re;it--){
        printf("%d -> ",*it);
        T_sort.erase(it);
    }
    printf("%d\n",*T_sort.begin());
    T_sort.erase(T_sort.begin());
}


int Graph::Diameter(){
    BFS(1);
    int farthest = 1;

    for(int i=1;i<=V;i++){
        if(Vertice[i].distance > Vertice[farthest].distance)
            farthest = i;
    }

    BFS(farthest);

    for(int i=1;i<=V;i++){
        if(Vertice[i].distance > Vertice[farthest].distance)
            farthest = i;
    }

    return Vertice[farthest].distance;
    
}

void Graph::printGraph(){
    for(int i=1;i<=V;i++){
        printf("Adj[%d] : ",i);
        for(list<int>::iterator it = Adjlist[i].begin();it!=Adjlist[i].end();it++){
            printf("%d ",*it);
        }
        printf("\n");
    }
}

int main(){
    int V;
    cin >> V;
    Graph G(V);
    int u,v;
    while(cin >> u >> v){
        G.addEdge_directed(u,v); // or undirected
    }
    return 0;
}