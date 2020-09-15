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

class Ans{
    public:
    int *r,*c;
    Ans(int n){
        r = new int[n];
        c = new int[n];
    }
};

Ans *Rod_Cutting_DP(int *p, int n){
    Ans *res = new Ans(n);
    res->r[0] = 0;
    res->c[0] = 0;
    for(int j=1;j<=n;++j){
        int max = -1;
        for(int i=1;i<=j;++i){
            if(max < p[i]+res->r[j-i]){
                max = p[i]+res->r[j-i];
                res->c[j] = i;
            }
        }
        res->r[j] = max;

    }
    return res;
}

int main(){
    int n;
    cin >> n;
    int *p = new int[n+1];
    for(int i=1;i<=n;++i)
        cin>>p[i];
    
    Ans *res = Rod_Cutting_DP(p,n);

    cout << res->r[n] << endl;

    return 0;
}