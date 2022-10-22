#include<bits/stdc++.h>
using namespace std; 

int main(){

    ifstream in("input.dat");
    int cnt = 0 ; 
    char arr[256] ;
    memset(arr, '*', sizeof(arr)) ;
    unordered_map<char, pair<int, int> > mp ; 
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
            mp[name] = {cnt,0}; 
            for(int i = 0 ; i<num && cnt<256 ; i++){
                arr[cnt++] = name ; 
            }
            mp[name].second = cnt-1 ; 
        }
        else if(op=='M'){
            int x = mp[name].second - mp[name].first-num; 
            if(x>=0){
                for(int i = x ; i>=0 ; i-- ){
                    arr[mp[name].second] = '*'; 
                    mp[name].second-- ; 
                }
            }
            else{
                cout<<"cannot put in continuation with initial memory , allocating at free space"<<endl;    
                for(int i = 0 ; i<num && cnt<256 ; i++){
                    arr[cnt++] = name ; 
                }    
            }
        }   
        else if(op=='D'){
            for(int i = mp[name].second ; i>=mp[name].first ; i--){
                arr[i] = '*'; 
            }
            mp.erase(name); 
        }
        else{
            cout<<"Index allocated to "<<name<<" are: "<<endl ;
            for(int i = mp[name].first ; i<=mp[name].second ; i++){
                cout<<i<<' ' ; 
            }
            cout<<endl; 
        }
    }

    cout<<"THe complete status of memory is as follows: "<<endl ;
    // to see total status of all blocks 
    for(int i = 0 ; i<256 ; i++){
        cout<<arr[i]<<' '; 
    }
    

    return 0 ; 
}
