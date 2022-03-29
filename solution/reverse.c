#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char* argv[]){
	FILE *FD, *FD2;
	int sizeFile;
    char* tmp;
    char checker_FD,checker_FD2;
	
    if (argc > 3){ //인자 개수가 3을 초과한 경우
        fprintf(stderr,"usage: reverse <input> <output>\n");
        exit(1);
    }
	
    if(argc == 1 ){ 
        FD = stdin;
        FD2 = stdout;
    }
	
    else{

        FD = fopen(argv[1],"r");  
        if(FD == NULL){       
            fprintf(stderr, "reverse: cannot open file '%s'\n",argv[1]);
            exit(1);
        }

        FD2 = fopen(argv[2],"w");
        if(FD2 == NULL){           
            fprintf(stderr, "reverse: cannot open file '%s'\n",argv[2]);
            exit(1);
        }

        while(1){
            if (feof(FD) == 0 && feof(FD2) == 0){
                checker_FD = fgetc(FD);
                checker_FD2 = fgetc(FD2);

                if (checker_FD != checker_FD2) // 읽어드린 정보가 하나라도 다르면 다른 파일이다.
                    break;
            }
            else{
                fprintf(stderr, "reverse: input and output file must differ\n");
                exit(1);
            }  
        }
        if(!strcmp(argv[1], argv[2])){ // 입력된 경로가 일치하면, 동일 파일로 간주함. 
            fprintf(stderr,"reverse: input and output file must differ\n");
            exit(1);
        }
    }

    
	// 입력된 두 인자 파일 경로가 동일한지 확인하는 부분
	// 조금 무식한 방법이지만, 두 인자 경로 파일 스트림을 열고 끝까지 읽어 드린다.
	// 둘중 하나가 먼저 끝나면 다른 파일로 인식하는 방식으로 전체 탐색이 진행되어야 해서 많은 연산을 요구한다.
	// 참고 : https://ddoddofather.tistory.com/entry/C%EC%96%B8%EC%96%B4-%EB%91%90%EA%B0%9C%EC%9D%98-%ED%8C%8C%EC%9D%BC%EC%9D%B4-%EA%B0%99%EC%9D%80%EC%A7%80%EB%A5%BC-%EA%B2%80%EC%82%AC%ED%95%B4%EB%B3%B4%EC%9E%90
   
	
	

    fseek(FD,0,SEEK_END);     // 파일의 맨 끝으로 이동
    sizeFile = ftell(FD);     // 파일 포인터의 현재 위치를 얻는다.
    char buffer[sizeFile+1];  // 확인된 파일 사이즈+1 만큼 버퍼를 할당. c언어 표준이 일부 바뀌면서 사이즈 고정이 아니여도 사용가능.
    char *result[sizeFile+1];  // 반환 역순 데이터를 저장.
	
    fseek(FD, 0, SEEK_SET);
    fread(buffer, sizeFile , 1 , FD); // 실제 파일 데이터를 읽음
    tmp = strtok(buffer, "\n");
	
    int i;
	for(i=0;; i++){
		if(tmp == NULL)				  // 자를 문자열이 없을때까지 반복함
			break;
		result[i] = tmp;
        tmp = strtok(NULL, "\n");     // 다음 문자열을 잘라서 포인터를 반환
	}

           
    for(int j=i-1; j>-1; j--){
        if(result[j] != NULL)
            fprintf(FD2,"%s\n",result[j]);
    }
            
    fclose(FD);
    fclose(FD2);
    return 0;
}

	// 파일을 역으로 뒤집어서 줄 단위로 출력하는 방법은 크게 두가지 방법이 존재한다.
	// 1. 파일 전체를 하나의 배열에 저장 후 공백 기준으로 분리, 반환된 포인터를 역으로 호출한다.
	//  - 위 방법은 구현은 쉬우나, 대용량 파일일 경우 할당이 실패할 가능성이 높다.
	//	  해당 과제는 1번 방식으로 해결하였으며, 
	// 
	// 2. 파일을 열고 해당 포인터를 파일 맨 끝으로 옮긴 다음, 역순으로 하나하나씩 읽어 드린다.
	//    만약 읽어 드린 문자가 \n이면 해당 (인덱스 주소 + 1)에 getline 함수를 호출하여 한줄을 출력한다.
	//	  getline 함수는 리눅스에서만 사용 가능하며, \n이나 널 문자까지 읽고 해당 정보를 버퍼에 저장해 준다.
	//    장점은, 문자열의 길이가 버퍼 사이즈 보다 길어지면 realloc을 호출하여 힙 사이즈를 늘려준다. 
	//    -> 즉 함수 사용이 끝나면 반드시 free를 호출해야 한다.
	//    - 물론 기한내에 작성을 실패했고, 디버깅 미숙으로 \n자체나 한 문자 라인이 출력되지 않는 문제가 발생하였다. ㅠㅠ
	//		구현 함수 reversContents는 아래 주석에 남긴다.
	//      참고 - https://stackoverflow.com/questions/6922829/read-file-backwards-last-line-first
	

/*void reversContents(char *argv[]){
    char c, b[1] = {'\n'};
    int head, tail;
    char *buffer;
    size_t size=0;
    int check_first = 1;
	
    FILE *FD = fopen(argv[1],"r");
    FILE *FD2 = fopen(argv[2],"w");

	fseek(FD,0,SEEK_END);
    while(ftell(FD) != 0){
        head = ftell(FD);
        fread(&c, 1, 1, FD);
        

        if(c == '\n'){
            getline(&buffer, &size, FD);
            fwrite(buffer, sizeof(char), strlen(buffer), FD2);
            
            if(check_first == 1){
                fwrite(b, sizeof(char), strlen(b), FD2);
                check_first=0;
            }
            
            fseek(FD, (tail - head)*(-1), SEEK_CUR);
            tail = head;
        }
        fseek(FD, -2, SEEK_CUR);
    }
    fseek(FD, 0, SEEK_SET);
    getline(&buffer, &size, FD);
    fwrite(buffer, sizeof(char), strlen(buffer), FD2);
}*/
