#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void reversContents(FILE* input, FILE* output);

int main(int argc, char* argv[]){
	FILE *fp, *fp2;
    char c,d;
	
    if (argc > 3){ //인자 개수가 3을 초과한 경우
        fprintf(stderr,"usage: reverse <input> <output>\n");
        exit(1);
    }
	
    else if(argc == 1){ 
        fp = stdin;
        fp2 = stdout;
    }
	
    else{

        fp = fopen(argv[1],"r");  
        if(fp == NULL){       
            fprintf(stderr, "reverse: cannot open file '%s'\n",argv[1]);
            exit(1);
        }

        fp2 = fopen(argv[2],"w");
        if(fp2 == NULL){           
            fprintf(stderr, "reverse: cannot open file '%s'\n",argv[2]);
            exit(1);
        }
	    

        while(1){
            if (feof(fp) == 0 && feof(fp2) == 0){
                c = fgetc(fp);
                d = fgetc(fp2);

                if (c != d) // 읽어드린 정보가 하나라도 다르면 다른 파일이다.
                    break;
            }
            else{
                fprintf(stderr, "reverse: input and output file must differ\n");
                exit(1);
            }  
        }
    }
    reversContents(fp, fp2);
    return 0;
}

void reversContents(FILE* input, FILE* output){
    int head, tail;
    char *buffer;
    size_t size=0;
    char c;
	
    FILE *fp = input;
    FILE *fp2 = output;

	fseek(fp,0,SEEK_END);

    while(ftell(fp)>0){
        head = ftell(fp);
        c =fgetc(fp);
        

        if(c == '\n'){
            getline(&buffer, &size, fp);
            fprintf(fp2, "%s", buffer);
            tail = ftell(fp);

            fseek(fp, (tail - head)*(-1), SEEK_CUR);
        }
        fseek(fp, -2, SEEK_CUR);
    }

    fseek(fp, 0, SEEK_SET);
    getline(&buffer, &size, fp);
    fprintf(fp2, "%s", buffer);
    free(buffer);
}
