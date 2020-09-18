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
# define infty 2147483647

using namespace std;


/* The trick in this priority queue is that 
   the parameter given to decrease-key function is "vertex no."
   However, in the priority queue, the index of the array 
   won't be same as the vertex number, 
   so we have to maintain another array to find the vertrx no. 
   in priority queue in O(1) time */
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

};


class Graph{
    public : 
    int V,*key,*pi,*inMST;
    vector<vector<pair<int,int>>> Adjlist; // pair< connected vertex, weight >
    Graph(int n){
        this->V = n;
        Adjlist.resize(n+1); // vertex no. from 1 ~ n
        key = new int[n+1];
        pi = new int[n+1];
        inMST = new int[n+1];
        for(int i=1;i<=n;i++){
            key[i] = infty;
            pi[i] = 0;
            inMST[i] = 0;
        }
    }
    
    void Add_edge(int u, int v, int w);
    int Prim(int start);
};

void Graph::Add_edge(int u, int v, int w){
    Adjlist[u].push_back({v,w});
    Adjlist[v].push_back({u,w});
}

int  Graph::Prim(int start){
    int MST_weight=0;
    // key[start] = 0;
    Priority_queue Q(this->V, this->key);
    Q.Decrease_key(start,0);

    while(!Q.Empty()){
        int u = Q.Extract_min();
        inMST[u] = 1;
        for(vector<pair<int,int>>::iterator it = Adjlist[u].begin();it!=Adjlist[u].end();it++){
            int v = it->first, w = it->second;
            if(!(inMST[v]) && key[v]>w){ // check if it's in MST and key > weight
                pi[v] = u;
                key[v] = w;
                Q.Decrease_key(v,w);
            }
        }
    }
    for(int i=1;i<=this->V;i++){
        MST_weight+=key[i];
    }
    return MST_weight;
}

int main(){
    int V;
    cin >> V;
    Graph G(V);
    int u,v,w;
    while(cin>>u>>v>>w){
        G.Add_edge(u,v,w);
    }
    printf("%d\n",G.Prim(1));
    printf("%d\n",G.Prim(2));
    printf("%d\n",G.Prim(3));
    printf("%d\n",G.Prim(4));
    
    return 0;
}