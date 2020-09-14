#include <iostream>
#include <string.h>
#include <vector>
#include <queue>
#include <set>
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
    vector<int> T_sort;
    vector<vertex> Vertice;
    vector<vector<int>> Adjlist;
    
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
        for(vector<int>::iterator it = Adjlist[u].begin();it!=Adjlist[u].end();it++){
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
    for(vector<int>::iterator it = Adjlist[u].begin();it!=Adjlist[u].end();it++){
        if(Vertice[*it].color == 'W'){
            Vertice[*it].pi = u;
            DFS_visit(*it);
        }
    }
    t++;
    Vertice[u].color = 'B';
    T_sort.push_back(u); // Topological sort
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

void Graph::Topological_Sort(int start){
    DFS(start);
    for(int i=V-1;i>0;i--)
        printf("%d -> ",T_sort[i]);
    printf("%d\n",T_sort[0]);
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
        for(vector<int>::iterator it = Adjlist[i].begin();it!=Adjlist[i].end();it++){
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
        G.addEdge_directed(u,v);
    }
    return 0;
}