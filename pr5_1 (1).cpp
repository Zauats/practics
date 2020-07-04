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
int renameFiles(const char* file){
    __pid_t pids[2048];
    __pid_t pid;
    DIR * dir;
    struct dirent * de;
    int nowPids = 0;
    
    char path[2048];
    strcpy(path, file);
    if ((dir = opendir(path)) == NULL){
        printf("Error opening %s: %s", path, strerror(errno));
        return 1;
    }

    errno = 0;
    de = readdir(dir);
    if (de == NULL && errno != 0){
        printf("Error %s: ", strerror(errno));
    }
    
    while(de) {
        char name[2048];
        strcpy(name, de->d_name);  

        char myPath[2048];
        strcpy(myPath, path);
        strcat(myPath, name);

        struct stat buff;
        int code = stat(myPath, &buff);
        if (code != -1){
            if(S_ISREG(buff.st_mode)) {  
                char path1[2048];
                char path2[2048];
                strcpy(path1, path);
                strcpy(path2, path);
                const char* name = de->d_name;
                char newName[2048] = {};
                strcat(path1, name);

                char simbols[10] = "aeiouy";

                int x = 0;
                for (int i = 0; i < strlen(name); i++){
                    bool isVowel = false;
                    for (int j = 0; j < 6; j++){
                        if (name[i] == simbols[j])
                            isVowel = true;
                    }
                    if (isVowel){
                        x++;
                    }
                    else{
                        newName[i - x] = name[i];
                    }
                }
                strcat(path2, newName);

                int code = rename(path1, path2); 
                if (code == -1) {
                    printf("%s", strerror(errno));
                }
                
            }
            else{
                if (S_ISDIR(buff.st_mode) && (strcmp(name, ".") != 0) && (strcmp(name, "..") != 0)) {
                    pid = fork(); 
                    if (pid == -1) {
                        printf("%s", strerror(errno));
                        exit(1);
                    }
                    if (pid == 0){
                        char newPath[2048];
                        strcat(path, name);
                        strcat(path, "/");
                        if ((dir = opendir(path)) == NULL){
                            printf("Error opening %s: %s", path, strerror(errno));
                            return 1;
                        }
                    }
                    else
                        pids[nowPids] = nowPids++;
                }
            }
        }
        else {
            printf("%s", strerror(errno));
        }

        errno = 0;
        de = readdir(dir);
        if (de == NULL && errno != 0){
            printf("Error %s: ", strerror(errno));
        }
    }

    
    int status = 1;
    for (int i = 0; i < nowPids - 1; i++){
        
        if (waitpid(pids[i], &status, 0) == -1){
            printf("%s", strerror(errno));
            exit(1);
        };
        if(WIFEXITED(status) == 0){
            printf("что-то пошло не так");
        }
    }
    
    closedir(dir);
    return 0;
}

int main(void){
    renameFiles("/home/zauats/Рабочий стол/tests2/");
}