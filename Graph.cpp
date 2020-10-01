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

struct Priority_queue{
    int size;
    int *vertexIndex; // the index in priority queue is not same as the vertex no.
    vector<pair<int,int>> pq; // < key , vertex no. >

    int Leftchild(int i){return 2*i;}

    int Parent(int i){return i/2;}

    void Heapify(int i){
        int j = Leftchild(i);
        pair<int,int> key = pq[i];
        while(j<=size){// leftchild exist
            if(j+1 <= size) {// rightchild exist
                if(pq[j].first>pq[j+1].first)
                    j++;
            }
            /* j = index of min{lchild,rchild} */
            if(key.first>pq[j].first){ // arr[j] move to index i
                vertexIndex[pq[j].second] = i;
                pq[i]= pq[j];
            }else // arr[i] is already minimum
                break;
            i = j;
            j = Leftchild(i);
        }
        vertexIndex[key.second] = i;
        pq[i]= key;
    }
    
    Priority_queue(int n, int *key){ // make heap
        this->size = n;
        vertexIndex = new int[n+1];
        pq.push_back({0,0});
        for(int i=1;i<=n;i++){
            vertexIndex[i] = i;
            pq.push_back({key[i],i});
        }
        /* make heap */
        for(int i=Parent(n);i>0;i--){
          Heapify(i);
        }
    }

    void Decrease_key(int i, int k){
        int x = vertexIndex[i]; // converting vertex no. to priority queue index
        pq[x].first = k;
        pair<int,int> key = pq[x];

        if(x == 1) // decrease-key of root, dont need to modify
            return;
        
        while(x>0){
            int p = Parent(x);
            if(pq[p].first > k){
                vertexIndex[pq[p].second] = x;
                pq[x] = pq[p];
            }else
                break;
            x = p;
        }
        vertexIndex[i] = x;
        pq[x] = key;
    }

    bool Empty(){
        return size == 0;
    }
    int Extract_min(){
        int min = pq[1].second;
        vertexIndex[pq[size].second] = 1;
        pq[1] = pq[size];
        this->size--;
        Heapify(1);
        return min;
    }

    void printQueue(){
        for(int i=1;i<=5;i++)
            printf("%d\n",pq[i].first);
    }
};

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
    vector<list<pair<int,int>>> w_Adjlist; // <vertex, weight>
    
    Graph(int n){
        this->V = n;
        this->t = 0;
        Vertice.resize(n+1);
        Adjlist.resize(n+1); // the vertex no. could be 1~n, not including 0
        w_Adjlist.resize(n+1); // the vertex no. could be 1~n, not including 0
    }
    void addEdge_undirected(int u, int v);
    void addEdge_directed(int u, int v);
    void addEdge_undirected_weighted(int u, int v, int w);
    void addEdge_directed_weighted(int u, int v, int w);
    void BFS(int start);
    void DFS_visit(int u);
    void DFS(int start);
    bool Relax(int u, int v, int w);
    void Dijkstra(int start);
    void SCC();
    void Topological_Sort(int start);
    int Diameter();
    void printGraph();
    void printGraph_weighted();
};

void Graph::addEdge_undirected(int u, int v){
    Adjlist[u].push_back(v);
    Adjlist[v].push_back(u);
}

void Graph::addEdge_directed(int u, int v){
    Adjlist[u].push_back(v);
}

void Graph::addEdge_undirected_weighted(int u, int v, int w){
    w_Adjlist[u].push_back({v,w});
    w_Adjlist[v].push_back({u,w});
}

void Graph::addEdge_directed_weighted(int u, int v, int w){
    w_Adjlist[u].push_back({v,w});
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

bool Graph::Relax(int u, int v, int w){
    if(Vertice[v].distance > Vertice[u].distance + w){
        Vertice[v].distance = Vertice[u].distance + w;
        return true;
    }
    return false;
}

void Graph::Dijkstra(int start){ // still testing
    int *key = new int[V];
    for(int i=1;i<=V;i++){
        key[i] = infty;
        Vertice[i].number=i;
        Vertice[i].distance = infty;
        Vertice[i].pi = -1;
    }
    Priority_queue Q(V,key);
    Vertice[start].distance = 0;
    Q.Decrease_key(start,0);
    while(!Q.Empty()){
        int u = Q.Extract_min();
        for(list<pair<int,int>>::iterator it=w_Adjlist[u].begin(), end=w_Adjlist[u].end();it!=end;it++){
            int v = it->first, w = it->second;
            if(Relax(u,v,w)){
                Q.Decrease_key(it->first,Vertice[v].distance);
                Vertice[it->first].pi = u;
            }
        }
    }
    for(int i=1;i<=V;i++){
        printf("%d : %d\n",Vertice[i].number,Vertice[i].distance);
    }
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

void Graph::printGraph_weighted(){
    for(int i=1;i<=V;i++){
        printf("Adj[%d] : ",i);
        for(list<pair<int,int>>::iterator it = w_Adjlist[i].begin();it!=w_Adjlist[i].end();it++){
            printf("%d ",it->first);
        }
        printf("\n");
    }
}

int main(){
    int V;
    cin >> V;
    Graph G(V);
    int u,v,w;
    while(cin >> u >> v >> w){
        G.addEdge_directed_weighted(u,v,w); // or undirected
    }
    G.Dijkstra(1);

    return 0;
}