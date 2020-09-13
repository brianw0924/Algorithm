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

class Graph{
    public : 
    int V; // vetex number
    vector<pair<int,pair<int,int> > > Edge; // < weight, <vertex 1,vertex 2> >
    
    Graph(int n){this->V = n;}
    void Add_edge(int u,int v,int w);
    int Kruskal();
};

struct DisjointSet{
    int *parent,*rank,n;
    DisjointSet(int n){// initialization
        this->n = n;
        rank = new int[n+1]; // vertex no. is 1~n
        parent = new int[n+1];
        for(int i=1;i<=n;i++){
            parent[i] = i; // root points to itself
            rank[i] = 0; // we use height as rank
        }
    }

    int Find_Halving(int x) { // similar to path compression
    while(parent[x] != x)
        x = parent[x] = parent[parent[x]];
    return x;
    }

    void Union_by_rank(int x, int y){ // rank : tree height
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
    sort(Edge.begin(),Edge.end()); // sort the edge by the weight
    vector<pair<int,pair<int,int> > >::iterator it=Edge.begin();
    DisjointSet S(this->V); // to check if the coming edge will form cycle or not
    while(edgeCount != E){ // in tree, E=V-1
        int u = S.Find_Halving(it->second.first), v = S.Find_Halving(it->second.second);
        if(u!=v){ // these 2 vertices are not in same set, so it won't form cycle in MST
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