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

class Ans {
    public :
    int max,left,right;
};

Ans MaximumSubarray(vector<int> A,int size){
    Ans res;
    int left=0,right=0,r;
    res.max = A[0];
    r = A[0];
    res.left = 0;
    res.right = 0;
    // cout << left << endl;
    for(int i=1;i<size;++i){
        // cout << "r+A[i] = "<< r+A[i];
        // cout << " , A[i] = "<< A[i] << endl;
        // cout << "left = "<< left << endl;
        if(r+A[i] < A[i]){ // 只有自己
            r = A[i];
            left = i;
            right = i;
        }else{ // 往左延伸
            r = r + A[i];
            right = i;
        }
        if(r > res.max){
            res.max = r;
            res.left = left;
            // cout << "LEFT = "<< left << endl;
            res.right = right;
        }
    }
    return res;
}
int main(){

    int size,a;
    cin >> size;
    vector<int> v(size);
    for(int i=0;i<size;++i){
        cin >> v[i];
    }
    Ans res = MaximumSubarray(v,size);
    cout << res.max << ", " << res.left << "~" << res.right <<endl;
    return 0;
}