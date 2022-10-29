#include<bits/stdc++.h>
using namespace std; 

void Sort(int initialHeadPos, vector<int> &sequence, int i){
    for(int k = i ; k<sequence.size() ; k++){
        for(int j = k+1 ; j<sequence.size() ; j++){
            if(abs(sequence[k]-initialHeadPos) > abs(sequence[j]-initialHeadPos)) swap(sequence[k], sequence[j]); 
        }
    }
}

void SSTF(
    int avgSeekTime, 
    int initialHeadPos, 
    vector<int> sequence     
){// shortest seek time first

    int s = 0 ; 
    for(int i = 0 ; i<sequence.size() ; i++){
        Sort(initialHeadPos, sequence, i);
        s += abs(sequence[i] - initialHeadPos); 
        initialHeadPos = sequence[i];
    }
    int totalSeekTime = s*avgSeekTime; 
    cout<<"TOTAL SEEK TIME IN SSTF is : "<<totalSeekTime<<endl ; 
}

void FCFS(
    int avgSeekTime, 
    int initialHeadPos, 
    vector<int> &sequence
)
{
    // First come first serve 
    int s = 0 ; 
    for(int i = 0 ;  i<sequence.size() ; i++){
        s += abs(sequence[i] - initialHeadPos); 
        initialHeadPos = sequence[i]; 
    }
    int totalSeekTime = s*avgSeekTime ;
    cout<<"TOTAL SEEK TIME IN FCFS : "<<totalSeekTime<<endl ;
}

void SCAN(
    int avgSeekTime, 
    int initialHeadPos, 
    vector<int> sequence
)
{
    // SCAN ALGO for processing requests
    int lastIndexSmaller = -1 ; 
    for(int i = 0 ; i<sequence.size() ; i++){
        if(sequence[i]>initialHeadPos){
            for(int j = i+1 ; j<sequence.size() ; j++){
                if(sequence[j]<initialHeadPos){
                    swap(sequence[i], sequence[j]) ;
                    break ; 
                }
            }
        }
    }
    for(int i = 0 ; i<sequence.size() ; i++){
        if(sequence[i] > initialHeadPos){
            lastIndexSmaller = i-1 ;
            break ;  
        }
    }

    if(lastIndexSmaller != -1){
        sort(sequence.begin(), sequence.begin()+lastIndexSmaller+1) ;
        reverse(sequence.begin(), sequence.begin()+lastIndexSmaller+1);
    }
    sort(sequence.begin()+lastIndexSmaller+1, sequence.end());

    int s= 0 ; 
    for(int i= 0  ;i<sequence.size() ; i++){
        s += abs(initialHeadPos-sequence[i]);
        initialHeadPos = sequence[i] ;
        if(lastIndexSmaller==i) initialHeadPos = 0 ; 
    }
    cout<<"TOTAL SEEK TIME IN SCAN IS : "<<s*avgSeekTime<<endl; 
}

void CSCAN(
    int avgSeekTime, 
    int initialHeadPos, 
    vector<int> sequence
)
{
    int lastIndexGreater = -1 ; 
    for(int i = 0 ; i<sequence.size() ; i++){
        if(sequence[i] < initialHeadPos){
            for(int j = i+1 ; j<sequence.size() ; j++){
                if(sequence[j] > initialHeadPos){
                    swap(sequence[i], sequence[j]) ;
                    break ; 
                }
            }
        }
    }

    for(int i = 0 ; i<sequence.size() ; i++){
        if(sequence[i] < initialHeadPos){
            lastIndexGreater = i-1 ;
            break ;  
        }
    }

    if(lastIndexGreater != -1){
        sort(sequence.begin(), sequence.begin()+lastIndexGreater+1) ;
    }
    sort(sequence.begin()+lastIndexGreater+1, sequence.end());

    int s= 0 ; 
    for(int i= 0  ;i<sequence.size() ; i++){
        s = s + (sequence[i] - initialHeadPos) ;
        if(lastIndexGreater==i) initialHeadPos = 0 ;
        else initialHeadPos = sequence[i];  
    }
    cout<<"TOTAL SEEK TIME IN CSCAN IS : "<<s*avgSeekTime<<endl; 
}

int main(){

    fstream file;
    string filename = "input.txt"; 
    file.open(filename.c_str());
    int num_cylinders, num_sectors, bytes_per_sector, rpm, avgSeekTime, initialHeadPos ;


    file>>num_cylinders>>num_sectors>>bytes_per_sector>>rpm>>avgSeekTime>>initialHeadPos ;
    vector<int> sequence; 
    int n;
    while(file>>n){
        sequence.push_back(n);
    }

    int averageRotDelay = 60000/rpm ; 

    FCFS(avgSeekTime, initialHeadPos, sequence);
    cout<<"Average Rotational Delay is "<<averageRotDelay<<"ms"<<endl;  
    SSTF(avgSeekTime, initialHeadPos, sequence); 
    cout<<"Average Rotational Delay is "<<averageRotDelay<<"ms"<<endl;  
    SCAN(avgSeekTime, initialHeadPos, sequence); 
    cout<<"Average Rotational Delay is "<<averageRotDelay<<"ms"<<endl;  
    CSCAN(avgSeekTime, initialHeadPos, sequence); 
    cout<<"Average Rotational Delay is "<<averageRotDelay<<"ms"<<endl;  
    return 0;
}

// unit of time will be same as of avg seek time 
// avg rotational delay will remain same as it depends on rpm 
