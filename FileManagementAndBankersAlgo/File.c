#include <stdio.h>
#include <string.h>
#include <dirent.h> 

void traversePath(char *path, int i);


int main()
{
   
    char name[100000]; 
    printf("Enter path to list files: ");
    scanf("%s", name);

    traversePath(name,0);

    return 0;
}

void traversePath(char *basePath, int i)
{
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);

   
    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) 
        {
            for(int k =  0 ; k<i ; k++){
                printf("   "); 
            }
            printf("|__%s\n", dp->d_name);
            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);

            traversePath(path,i+1);
        }
    }

    closedir(dir);
}
