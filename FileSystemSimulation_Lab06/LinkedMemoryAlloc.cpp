#include<bits/stdc++.h>
using namespace std; 
#define SIZEALLOCATED 256

class Node{
    public:
        char val ; 
        Node* next ;

    Node(char v){
        val = v ; 
        next = NULL ; 
    } 
};

void InsertAtEnd(Node* &head , Node* &tail, char val, int k){
    // cout<<"CALLING RECU"<<endl ;
    if(k==0) return ;
    
    if(!head){
        head = tail = new Node(val); 
        InsertAtEnd(head, tail, val, k-1); 
    }

    else{
        tail->next = new Node(val); 
        tail = tail->next ; 
        InsertAtEnd(head, tail, val, k-1); 
    }
}   

void DeleteNodes(Node* &head , char val ){
    Node*temp = head ; 
    while(temp && temp->val!=val){
        temp = temp->next ; 
    }
    if(temp==NULL) return ; 

    Node*temp1 = temp ; 
    while(temp1 && temp1->val==val){
        temp1 = temp1->next ;
    }
    if(!temp1){
        temp = NULL ; 
        return;
    }
    temp->next = temp1; 
}

void PrintNodes(Node* head, char val){
    while(head){    
        while(head && head->val!=val){
            // cout<<"NOT FOUND"<<endl ;
            head = head->next ;
        }

        while(head && head->val==val){
            
            cout<<val<<' '<<head<<endl ;
            head = head->next ; 
        }
    }
        
}   

int main(){
    ifstream in("input.dat");
    Node*head = NULL , *tail = NULL ; 
    int szOccupied = 0 ;   
    unordered_map< char, int> char_to_size ;
    while(!in.eof()){
        string text ; 
        getline(in, text) ;
        char name = text[0] ;
        char op = text[2]; 
        int num = 0 ; 
        for(int i = 4 ; i<text.length() && text[i]!=' ' ; i++){
            num = num*10 + int(text[i]-'0'); 
        }

        if(op=='C'){
            if(szOccupied+num > SIZEALLOCATED){
                cout<<"Not enough memory to allocate "<<num<< " blocks "<<endl ;   
            }
            else{
                char_to_size[name] = num ; 
                szOccupied += num ; 
                InsertAtEnd(head, tail, name, num); 
            }
        }
        else if(op=='M'){
            if(num>char_to_size[name]){
                InsertAtEnd(head, tail, name, num-char_to_size[name]); 
            }
            else{
                int x = char_to_size[name] - num ; 
                Node*temp = head ; 
                while(temp && temp->val!=name){
                    temp = temp->next ;
                }
                for(int i = 1 ; i<num ; i++){
                    temp = temp->next ;
                }

                while(temp && temp->val==name){
                    temp->val = '*' ; 
                    temp = temp->next ;
                }
            }
        }   
        else if(op=='D'){
            DeleteNodes(head, name) ;
        }
        else{
            PrintNodes(head, name);
        }
    }
    cout<<endl<<"MEMORY LAYOUT: "<<endl ;
    while(head){
        cout<<head->val<<' ' ; 
        head = head->next ;
    }
    cout<<endl ;
    return 0 ; 
}
