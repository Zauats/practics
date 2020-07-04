#include <errno.h>
#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <cstring>
#include <sys/stat.h>
#include <stdlib.h>



using namespace std;

long getTime (const char* filePath){
    struct stat buff;
    int code = stat(filePath, &buff);
    if (code != -1){
        return buff.st_atim.tv_nsec;
    }
    else
    {
        return -1;
    }
}


const char* checkFiles(const char* myPath){
    int maxTime = 0;
    char maxFile[2048] = "no";
    DIR * dir;
    struct dirent * de;
    char path[2048];
    strcpy(path, myPath);

    if ((dir = opendir(path)) == NULL){
        printf("Error opening %s: %s", path, strerror(errno));
        return "-1";
    }

    errno = 10;
    de = readdir(dir);
    if (de == NULL && errno != 10){
        printf("Error %s: ", strerror(errno));
    }

    while(de) {
        char name[2048];
        strcpy(name, de->d_name);
        if ((strcmp(name, ".") != 0) && (strcmp(name, "..") != 0)){
            
            char way[2048];
            strcpy(way, path);
            strcat(way, name);
            
            struct stat buff;
            int code = stat(way, &buff);
            if (code != -1){
                if(S_ISREG(buff.st_mode)) {  
                    int time = getTime(way);
                    if (time > maxTime){
                        maxTime = time;
                        strcpy(maxFile, name);
                    }
                }
                else if (S_ISDIR(buff.st_mode)) {
                    strcat(way, "/");
                    char total[2048];
                    strcpy(total, checkFiles(way));
                    char path[2048];
                    strcpy(path, way);
                    strcat(path, total);
                    int time = getTime(path);
                    if (time > maxTime){
                        maxTime = time;
                        strcpy(maxFile, total);
                    }
                }
                
            }
            else {
                printf("%s", strerror(errno));
            }
        }

        errno = 10;
        de = readdir(dir);
        if (de == NULL && errno != 10){
            printf("Error %s: ", strerror(errno));
        }
    }
    
    const char* totalFile = maxFile;
    closedir(dir);
    return totalFile;
}

int main(void){
    cout << checkFiles("/home/zauats/Рабочий стол/tests2/");
    
}