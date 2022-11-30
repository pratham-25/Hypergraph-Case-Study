#include<bits/stdc++.h>
using namespace std;

class Node{
    pair<int,int> data;
    vector<Node*> child;
public:
    Node(){
        data.first = 0;
        data.second = 0;
        child.resize(0);
    }

    Node(int data,int val){
        this->data.first = data;
        this->data.second = val;
    }

    vector<Node*> getChild(){
        return child;
    }

    int getValue(){
        return data.first;
    }
    
    friend class Tree;
};

class Tree{
    Node *root;
public:
    Tree(){
        root = NULL;
    }

    Node* create_Root(int key,int val){
        root = new Node(key,val);
        return root;
    }

    Node* getRoot(){
        return root;
    }

    pair<int,int> getRootValue(){
        return root->data;
    }
    
    vector<int> getPath(Node *parent,int key){
        if(parent->data.first == key){
            vector<int> path;
            path.push_back(key); 
            return path;
        }

        for(Node* child : parent->child){
            vector<int> ptc = getPath(child,key);
            if(ptc.size() > 0){
                ptc.push_back(parent->data.first);
                return ptc;
            }
        }

        return {};
    }

    bool find(Node *parent,int key,Node *&ele,Node *&par){
        if(parent->data.first == key){
            ele = parent;
            return true;
        }

        for(Node* child:parent->child){
            bool check = find(child,key,ele,par);
            if(check){
                par = child;
                return true;
            }
        }

        ele = nullptr;
        return false;
    }


    void add(int parent,int key,int val){
        Node *ele,*par;
        if(find(root,parent,ele,par)){
            Node *child = new Node(key,val);
            ele->child.push_back(child);
        }
        else{
            cout << "Parent does not exists" << "\n";
            return;
        }
    }

    void remove(Node* parent,int key){
        Node *ele,*par;
        if(find(parent,key,ele,par)){
            for(int i = 0;i < par->child.size();i++){
                if(par->child[i]->data.first == key){
                    par->child.erase(par->child.begin() + i);
                    break;
                }
            }
        }
        else{
            cout << "Parent does not exists" << "\n";
            return;
        }
    }

    void display(Node *t){
        cout << t->data.first << "->" << t->data.second << " :- ";
        for(Node* child : t->child){
            cout << child->data.first << "->" << child->data.second << ", ";
        }
        cout << "\n";

        for(Node* child : t->child){
            display(child);
        }
    }

};


