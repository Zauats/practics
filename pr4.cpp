#include <iostream>
#include <fstream>
#include <dirent.h>
#include <cstring>
#include <sys/stat.h>


using namespace std;

int main(int argc, const char* argv[]){
    if (argc == 4){
        ifstream file1;
        file1.open(argv[1]);
        if(!file1.is_open()) {
            printf("Error opening1: %s", strerror(errno));
            cout << "\n";
            exit(1);
        }

        ifstream file2;
        file2.open(argv[2]);
        if(!file2.is_open()) {
            printf("Error opening2: %s", strerror(errno));
            cout << "\n";
            exit(1);
        }


        ofstream file3;
        file3.open(argv[3]); 
        if(!file3.is_open()) {
            printf("Error opening3: %s", strerror(errno));
            cout << "\n";
            exit(1);
        }


        struct stat fi;
        stat(argv[1], &fi);
        
        char data;
        
        for (int i = 0; i < fi.st_size; i++){
            file1.read(&data, 1);
            if(!file1.good()){
                printf("Error reading: %s", strerror(errno));
                cout << "\n";
                exit(1);
            }
            else{
                file3.write(&data,1);
                if(!file3.good()){
                    printf("Error writing: %s", strerror(errno));
                    cout << "\n";
                    exit(1);
                }
                
            }
        }     

        stat(argv[2], &fi);
        
        for (int i = 0; i < fi.st_size; i++){
            file2.read(&data, 1);
            if(!file2.good()){
                printf("Error reading: %s", strerror(errno));
                cout << "\n";
                exit(1);
            }
            else{
                file3.write(&data,1);
                if(!file3.good()){
                    printf("Error writing: %s", strerror(errno));
                    cout << "\n";
                    exit(1);
                }
            }
        }                 

        
        file1.close();
        file2.close();
        file3.close();
    }
    else{
        cout << "Error\n";
    }
    
    return 0;
}