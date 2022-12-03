#include<bits/stdc++.h>
#include "tree.h"
using namespace std;

map<int,vector<int>> getEOS(int n,int m,vector<vector<int>> &a){
    map<int,vector<int>> mpp;
    for(int i = 1;i <= m;i++){
        vector<int> tmp;
        mpp[i] = tmp;
        for(int j = 1;j <= n;j++){
            if(a[j][i] == 1){
                mpp[i].push_back(j);
            }   
        }
    }

    return mpp;
}

map<int,vector<int>> getAES(int n,int m,vector<vector<int>> &a){
    map<int,vector<int>> mpp;
    for(int i = 1;i <= n;i++){
        vector<int> tmp;
        mpp[i] = tmp;
        for(int j = 1;j <= m;j++){
            if(a[i][j] == 1){
                mpp[i].push_back(j);
            }
        }
    }

    return mpp;
}

map<int,vector<int>> getAOS(int n,int m,map<int,vector<int>> &eos){
    map<int,vector<int>> mpp;
    for(int i = 1;i <= n;i++){
        vector<int> tmp;
        for(auto x : eos){
            for(auto v : x.second){
                if(find(x.second.begin(),x.second.end(),i) != x.second.end()){
                    if(v != i && find(tmp.begin(),tmp.end(),v) == tmp.end()){
                        tmp.push_back(v);
                    }
                }
            }
        }
        mpp[i] = tmp;
    }

    return mpp;
}

map<int,vector<array<int,3>>> getAssMEW(int n,int m,vector<int> &ew,map<int,vector<int>> &eos,map<int,vector<int>> &aos){
    map<int,vector<array<int,3>>> mpp;
    for(int i = 1;i <= n;i++){
        mpp[i] = {};
        for(auto oj : aos[i]){
            int wt = INT_MAX,edge;
            for(int j = 1;j <= m;j++){
                if(i != oj && find(eos[j].begin(),eos[j].end(),i) != eos[j].end() && find(eos[j].begin(),eos[j].end(),oj) != eos[j].end()){
                    if(wt > ew[j]){
                        wt = ew[j];
                        edge = j;
                    }
                }
            }
            array<int,3> a = {oj,edge,wt};
            mpp[i].push_back(a);
        }
    }

    return mpp;
}

int cost(int parent,int oj,map<int,vector<array<int,3>>> &assMew,map<int,int> &f){
    int minCost = f[parent];
    for(auto x : assMew[parent]){
        if(x[0] == oj) minCost += x[2];
    } 

    return minCost;
}

int main(){

    int n,m; // n -> vertices, m -> edges

    cout << "Enter number of vertices :- ";
    cin >> n;
    cout << "Enter number of edges :- ";
    cin >> m;

    vector<vector<int>> a(n+1,vector<int>(m+1,-1));
    vector<int> ew(m + 1,1);

    for(int i = 1;i <= n;i++){
        for(int j = 1;j <= m;j++){
            cout << "Value for o" << i << " -> e" << j << " :- ";
            cin >> a[i][j]; 
        }
    }

    for(int i = 1;i <= m;i++){
        cout << "Enter weight for " << i << " edge :- ";
        cin >> ew[i];
    }

    map<int,vector<int>> eos = getEOS(n,m,a);
    map<int,vector<int>> aes = getAES(n,m,a);
    map<int,vector<int>> aos = getAOS(n,m,eos);
    map<int,vector<array<int,3>>> assMew = getAssMEW(n,m,ew,eos,aos);

    cout << "\nEOS\n";
    for(auto ele : eos){
        cout << ele.first << " -> {";
        for(auto x : ele.second) cout << x << " "; 
        cout << "}\n";
    }

    cout << "\nAES\n";
    for(auto ele : aes){
        cout << ele.first << " -> {";
        for(auto x : ele.second) cout << x << " "; 
        cout << "}\n";
    }

    cout << "\nAOS\n";
    for(auto ele : aos){
        cout << ele.first << " -> {";
        for(auto x : ele.second) cout << x << " "; 
        cout << "}\n";
    }

    cout << "\nAssMEW\n";
    for(auto ele : assMew){
        cout << ele.first << " -> {";
        for(auto v : ele.second){
            cout << "<";
            for(int j = 0;j < 3;j++){
                cout << v[j] << ' ';
            }
            cout << ">, ";
        }
        cout << "}\n";
    }

    int currNode;
    map<int,Tree> branchingTree;
    map<int,map<int,int>> costs;
    for(int i = 1;i <= n;i++){
        vector<bool> canVisit(n+1,true);
        bool ExistExpandNode = true;
        map<int,int> f,g;
        
        f[i] = 0;
        g[i] = f[i];
        currNode = i;
        canVisit[i] = false;

        Tree ti;
        Node *ri = ti.create_Root(i,f[i]);
        int limit = 1;
        do{
            if(aos[currNode].size() == 0 || ExistExpandNode == false){
                canVisit[i] = false;
            }
            else{
                for(auto oj : aos[currNode]){
                    if(canVisit[oj]) f[oj] = cost(currNode,oj,assMew,f);
                    if(f[oj] < g[oj]){
                        ti.remove(ri,oj);
                    }
                    else{
                        if(canVisit[oj]) ti.add(currNode,oj,f[oj]);
                        canVisit[oj] = false;
                        g[oj] = f[oj];
                    }
                }
                ExistExpandNode = false;
                canVisit[currNode] = false;
            }

            ExistExpandNode = false;
            int mingi = INT_MAX;
            for(auto nodes : ri->getChild()){
                if(aos[nodes->getValue()].size() > limit){
                    ExistExpandNode = true;
                    if(mingi > g[nodes->getValue()]){
                        mingi = g[nodes->getValue()];
                        currNode = nodes->getValue();
                    }
                }
            }
            limit++;
        }while(ExistExpandNode);

        branchingTree[i] = ti;
        costs[i] = f;
    }

    cout << "\nCost 1-6 -> " << costs[1][6] << "\n";
    cout << "\nCost 5-2 -> " << costs[5][2] << "\n";
    
    return 0;
}

/*
-- Sample Test Case -- 

    Enter number of vertices :- 7
    Enter number of edges :- 4
    Value for o1 -> e1 :- 1
    Value for o1 -> e2 :- 0
    Value for o1 -> e3 :- 0
    Value for o1 -> e4 :- 0
    Value for o2 -> e1 :- 1
    Value for o2 -> e2 :- 1
    Value for o2 -> e3 :- 0
    Value for o2 -> e4 :- 0
    Value for o3 -> e1 :- 1
    Value for o3 -> e2 :- 1
    Value for o3 -> e3 :- 1
    Value for o3 -> e4 :- 0
    Value for o4 -> e1 :- 0
    Value for o4 -> e2 :- 0
    Value for o4 -> e3 :- 0
    Value for o4 -> e4 :- 1
    Value for o5 -> e1 :- 0
    Value for o5 -> e2 :- 0
    Value for o5 -> e3 :- 1
    Value for o5 -> e4 :- 0
    Value for o6 -> e1 :- 0
    Value for o6 -> e2 :- 0
    Value for o6 -> e3 :- 1
    Value for o6 -> e4 :- 0
    Value for o7 -> e1 :- 0
    Value for o7 -> e2 :- 0
    Value for o7 -> e3 :- 0
    Value for o7 -> e4 :- 0
    Enter weight for 1 edge :- 3
    Enter weight for 2 edge :- 2
    Enter weight for 3 edge :- 3
    Enter weight for 4 edge :- 1

    EOS
    1 -> {1 2 3 }
    2 -> {2 3 }
    3 -> {3 5 6 }
    4 -> {4 }

    AES
    1 -> {1 }
    2 -> {1 2 }
    3 -> {1 2 3 }
    4 -> {4 }
    5 -> {3 }
    6 -> {3 }
    7 -> {}

    AOS
    1 -> {2 3 }
    2 -> {1 3 }
    3 -> {1 2 5 6 }
    4 -> {}
    5 -> {3 6 }
    6 -> {3 5 }
    7 -> {}

    AssMEW
    1 -> {<2 1 3 >, <3 1 3 >, }
    2 -> {<1 1 3 >, <3 2 2 >, }
    3 -> {<1 1 3 >, <2 2 2 >, <5 3 3 >, <6 3 3 >, }
    4 -> {}
    5 -> {<3 3 3 >, <6 3 3 >, }
    6 -> {<3 3 3 >, <5 3 3 >, }
    7 -> {}

    Cost 1-6 -> 6

    Cost 5-2 -> 5
*/