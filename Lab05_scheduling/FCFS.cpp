#include <bits/stdc++.h>
using namespace std; 

class process{

public:
    int process_id ;
    int arrival_time ; 
    vector<int> burst ;
    int j = 0 ; 
    int cur_time = 0 ; 
};

int main(){
    ios_base:: sync_with_stdio(false);
    cin.tie(nullptr); 

    freopen("process.dat","r",stdin); 
    int num_processes ;
    cin>>num_processes ;

    map < int , process > mp ;   

    for(int i = 0 ; i<num_processes ; i++){
        int x1 , x2 ;
        // x1 -> process id
        // x2 -> arrival time 
        cin>> x1 >> x2 ;
        mp[x2].process_id = x1, mp[x2].arrival_time = x2 ; 
        int x ; 
        mp[x2].cur_time = mp[x2].arrival_time; 
        while(cin>>x) {
            mp[x2].burst.push_back(x); 
            if(x==-1){
                mp[x2].burst.pop_back();
                break ; 
            }
        }
    }

    // sort by arrival time 
    auto it = mp.begin() ; 
    int time = 0 ;

    queue <process> CPU ;
    queue <process> IO; 

    while(1){
        // cout<<"hello"<<endl;
        if(it!=mp.end() && time >= it->first ){
            // cout<<"hello1"<<endl;
            CPU.push(it->second) ;
            cout<< "process with arrival time "<<it->first<<" and process id "<< (it->second).process_id <<" entered at time "<<time<<endl; 
            it++;
            // cout<<"hello3"<<endl;
        }

        if(!CPU.empty()){
            process x = CPU.front() ; 
            // cout<<"hello2"<<endl;
            if(x.cur_time + x.burst[x.j] <= time ){
                CPU.pop() ; 
                x.cur_time += x.burst[x.j] ;
                (x.j)++ ; 
                if(x.j < x.burst.size()){
                    IO.push(x) ;
                    cout<< "process "<< x.process_id << " out of CPU queue and now in IO queue at time "<< time <<endl ;
                }
                else{
                    cout<<"process "<< x.process_id << " finished"<<endl ;
                }
            }
        }

        if(!IO.empty()){
            process x = IO.front() ; 
            if(x.cur_time + x.burst[x.j] <= time ){
                IO.pop() ; 
                x.cur_time += x.burst[x.j] ;
                (x.j)++ ; 
                if( x.j < x.burst.size()){
                    CPU.push(x) ;
                    cout<< "process "<< x.process_id << " out of IO queue and now in  CPU queue at time "<<time<<endl ;
                }
                else{
                    cout<<"process "<< x.process_id << " finished"<<endl ;
                }
            }
            
        }


        time++ ;
        if(it==mp.end() && CPU.empty() && IO.empty()){
            break ;
        } 
    }

    
    return 0; 
}
