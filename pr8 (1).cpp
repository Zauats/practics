#include <errno.h>
#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <cstring>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h> 
#include <sys/wait.h>



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


int main(void){
    pid_t pids[1024];
    int nowPid = 0;
    int pipes[1024][2];
    int nowPipe = 0;
    bool main = true;
    int i = 0;
    int processPipe;

    int maxTime = 0;
    char maxFile[2048] = "no";
    DIR * dir;
    struct dirent * de;
    char path[2048];
    strcpy(path, "/home/zauats/Рабочий стол/tests2/");

    if ((dir = opendir(path)) == NULL){
        printf("Error opening %s: %s", path, strerror(errno));
        return -1;
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
                        strcpy(maxFile, way);
                    }

                }
                else if (S_ISDIR(buff.st_mode)) {
                    
                    if (pipe(pipes[nowPipe]))
                    {
                        fprintf (stderr, "Pipe failed.\n");
                        return EXIT_FAILURE;
                    }
                    
                    pids[nowPid] = fork(); 
                    if (pids[nowPid] == -1) {
                        printf("%s", strerror(errno));
                        exit(1);
                    }
                    if (pids[nowPid] == 0){
                        processPipe = pipes[nowPipe][1];
                        nowPipe = 0;
                        nowPid = 0;
                        main = false;
                        strcat(path, name);
                        strcat(path, "/");
                        if ((dir = opendir(path)) == NULL){
                            printf("Error opening %s: %s", path, strerror(errno));
                            return 1;
                        }
                        
                    }
                    else{   
                        nowPid++;
                        nowPipe++;
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
    
    closedir(dir);
    int status = 1;
    for (int j = 0; j < nowPid; j++){
        if (waitpid(pids[j], &status, 0) == -1){
            printf("%s", strerror(errno));
            exit(1);
        };
        if(WIFEXITED(status) == 0){
            printf("Error");
        }
    }
    char inbuf[1024];
    for (int j = 0; j < nowPipe; j++){
        if(read(pipes[j][0], &inbuf, 100) == -1){
            printf("Error %s: ", strerror(errno));
        };
        
        char myFileName[1024];
        strcpy(myFileName, inbuf);
        if (getTime(myFileName) > maxTime){
            strcpy(maxFile, myFileName);
        }
    }
    

    
    if (!main){
        if (write(processPipe, maxFile, 100) == -1){
            printf("Error %s: ", strerror(errno));
        }; 
        close(processPipe);
        
    }


    if (main){
       cout << "Ответ: " << maxFile << "\n";
    }
    
    return 1;
}