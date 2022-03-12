#include <stdio.h>
#include <errno.h>
#define MAX_LEN 256

int main(int argc, char* argv[]){
    FILE* FD;
    char contents[MAX_LEN];
    
    //1. if no input argument in main
    if(argc == 1) 
        return 0;
    
    for(int i=1; i < argc; i++){
        FD = fopen(argv[i], "r");
        
        if(FD == NULL){ 
            //2. error num : ENOENT(2) : no such file or directory.
            if(errno == ENOENT) 
                printf("wcat: cannot open file\n");

            return 1;
        }

        while( !feof(FD)){ //3. End-of-File indicator
            if(!fgets(contents, MAX_LEN , FD))
                break;
                
            printf("%s", contents);
        }
        fclose(FD);
    }
    return 0;
}
