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

using namespace std;

class Graph{
    public : 
    int V;
    vector<pair<int,pair<int,int> > > Edge;
    
    Graph(int n){this->V = n;}
    void Add_edge(int u,int v,int w);
    int Kruskal();
};

struct DisjointSet{
    int *parent,*rank,n;
    DisjointSet(int n){// 初始化
        this->n = n;
        rank = new int[n+1];
        parent = new int[n+1];
        for(int i=1;i<=n;i++){
            parent[i] = i; // root 指向自己
            rank[i] = 0;
        }
    }

    int Find_Halving(int x) {
    while(parent[x] != x)
        x = parent[x] = parent[parent[x]];
    return x;
    }

    void Union_by_rank(int x, int y){
        int rx = Find_Halving(x), ry = Find_Halving(y);
        if(rx == ry)
            return;
        else{
            if(rank[rx] > rank[ry]){
                parent[ry] = rx;
            }else if(rank[rx] == rank[ry]){
                parent[ry] = rx;
                rank[rx]++;
            }else
                parent[rx] = ry;
            }
        }
};

void Graph::Add_edge(int u, int v, int w){
    Edge.push_back({w,{u,v}});
}

int Graph::Kruskal(){
    int MST_weight=0, edgeCount=0, E = this->V - 1;
    sort(Edge.begin(),Edge.end());
    vector<pair<int,pair<int,int> > >::iterator it=Edge.begin();
    DisjointSet S(this->V);
    while(edgeCount != E){
        int u = S.Find_Halving(it->second.first), v = S.Find_Halving(it->second.second);
        if(u!=v){
            S.Union_by_rank(u,v);
            MST_weight+=it->first;
            edgeCount++;
        }
        it++;
    }

    return MST_weight;
}
int main(){
    int V;
    cin >> V;
    int u,v,w;
    Graph G(V);
    while(cin>>u>>v>>w){
        // printf("%d %d %d\n",u,v,w);
        G.Add_edge(u,v,w);
    }
    printf("%d\n",G.Kruskal());
    return 0;
}