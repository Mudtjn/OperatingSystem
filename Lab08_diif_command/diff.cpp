#include <bits/stdc++.h>
#include <fstream>
using namespace std;
int main(){
    ifstream f1, f2;
    f1.open("a.txt");
    f2.open("b.txt");
    string line1, line2;
    vector<string> fi1, fi2;
    while(getline(f1, line1)){
        fi1.push_back(line1);
    }
    while(getline(f2, line2)){
        fi2.push_back(line2);
    }
    int n = fi1.size() ; 
    int m = fi2.size() ;
    vector<vector<int>> arr = vector<vector<int>>(fi1.size() + 1, vector<int>(fi2.size() + 1, 0));
    for (int i = 1; i < arr.size(); i++){
        for (int j = 1; j < arr[0].size(); j++){
            if (fi1[i - 1] == fi2[j - 1]){
                arr[i][j] = arr[i - 1][j - 1]+1;                
            }
            else 
                arr[i][j] = max(arr[i][j], max(arr[i-1][j], arr[i][j-1])); 
        }
    }

    vector< pair<int,int> > LCS(arr[n][m]);  // this will determine all the similar indices 
    // store the indices in pair
    int i = n , j = m, index = arr[n][m]-1 ;
    while(i>0 && j>0 ){
        if(fi1[i-1] == fi2[j-1]){
            LCS[index] = {i-1,j-1};
            i-- , j--;
            index-- ;  
        }
        else if(arr[i-1][j] > arr[i][j-1]){
            i-- ; 
        }
        else{
            j-- ; 
        }
    } 

    string str1 = "" , str2 = ""  ;
    int s1 = LCS[0].first, s2 = LCS[0].second ;
    if(s1 && s2){
        for(int i = 0 ; i<LCS[0].first ; i++){
            str1 += ("!" + fi1[i] + "\n") ;
        }
        for(int i = 0 ; i<LCS[0].second ; i++){
            str2 += ("!" + fi2[i] + "\n") ;
        }
    }

    else if(s1==0 && s2 != 0 ){
        for(int i = 0 ; i<LCS[0].second ; i++){
            str2 += ("+" + fi2[i] + "\n") ;
        }        
    }

    else if(s1 != 0 && s2==0){
        for(int i = 0 ; i<LCS[0].first ; i++){
            str1 += ("-" + fi1[i] + "\n") ;
        } 
    }
    str1 += (fi1[LCS[0].first]+"\n"); 
    str2 += (fi2[LCS[0].second] + "\n");

    for(int j = 1 ; j<LCS.size() ; j++){

        int t1 = LCS[j].first - LCS[j-1].first-1 ;
        int t2 = LCS[j].second - LCS[j-1].second-1 ;

        if(t1 && t2){
            for(int i = LCS[j-1].first + 1 ; i<LCS[j].first ; i++){
                str1 += ("!" + fi1[i] + "\n") ;
            }
            for(int i = LCS[j-1].second + 1 ; i< LCS[j].second ; i++){
                str2 += ("!" + fi2[i] + "\n") ;
            }            
        }

        else if(t1!=0 && t2==0){
            for(int i = LCS[j-1].first +1 ; i<LCS[j].first ; i++){
                str1 += ("-" + fi1[i] + "\n") ;
            }   
        }

        else if(t1==0 && t2!=0){
            for(int i = LCS[j-1].second+1 ; i < LCS[j].second ; i++){
                str2 += ("+" + fi2[i] + "\n") ;
            }   
        }

        str1 += (fi1[LCS[j].first]+ "\n");
        str2 += (fi2[LCS[j].second]+ "\n"); 
    }

    int n3 = LCS.size()-1; 

    int n1 = fi1.size() - LCS[n3].first  -1; 
    int n2 = fi2.size() - LCS[n3].second -1; 

    if(n1 && n2){
        for(int i = LCS[n3].first + 1 ; i < fi1.size() ; i++){
            str1 += ("!" + fi1[i] + "\n") ;
        }
        for(int i = LCS[n3].second ; i < fi2.size() ; i++){
            str2 += ("!" + fi2[i] + "\n") ;
        }            
    }

    else if(n1!=0 && n2==0){
        for(int i = LCS[n3].first +1 ; i < fi1.size() ; i++){
                str1 += ("-" + fi1[i] + "\n") ;
        }   
    }

    else if(n1 == 0 && n2!=0){
        for(int i = LCS[n3].second +1 ; i < fi2.size() ; i++){
                str2 += ("+" + fi2[i] + "\n") ;
        }
    }

    cout<<str1<<"----------------------------"<<endl<<str2 ; 

    return 0; 
}
