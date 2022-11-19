#include<bits/stdc++.h>
#include<dirent.h>

using namespace std ;

vector<string>ans;
void ReadDirectory(string basePath){
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (basePath.c_str())) != NULL) {
    /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL) {
            // printf ("%s\n", ent->d_name);
            if(strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0){
                string s = basePath + '/' + ent->d_name ;
                ans.push_back(s); 
                ReadDirectory(s) ;
            }
        }
        closedir (dir);
    } else {
    /* could not open directory */
        return ; 
    }
}

int main(){

    string basePath = "/home/quaternion/OS/OperatingSystem/Lab09_DelDuplicates" ;
    ReadDirectory(basePath);
    for(auto x: ans){
        cout<<x<<endl ;
     }

    unordered_map<size_t, string> h; 

    for(int i = 0 ; i<ans.size() ; i++){
        ifstream f(ans[i]) ;
        string line1, s ;
        vector<string> fi1 ;
        while(getline(f, line1)){
            fi1.push_back(line1);
        }
        for(auto x: fi1)
        {
            s += x ; 
        }

        size_t hsh = hash<string>{}(s); 
        if(h.count(hsh) != 0){
            cout<<ans[i]<<" IS COPY "<<endl ;
            remove(ans[i].c_str()); 
        }
        else {
            h[hsh] = ans[i] ; 
        }
    }


    return 0; 
}


