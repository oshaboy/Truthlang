#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
void syntax_error(){
	fprintf(stderr,"Syntax Error!\n");
	exit(1);
}
char * falsey="#include <stdio.h>\nint main(void){printf(\"0\\n\");return 0;}\n";
char * truey="#include <stdio.h>\nint main(void){while(1)printf(\"1\");}\n";
int main(int argc, char * argv[]){
	if (argc<=1 || (strcmp(argv[1],"--help")==0)){
		printf("Usage: truthlang infile [outfile]\n");
		return 0;
	}
	char src_code[65536];
	FILE * src_file=fopen(argv[1],"r");
	int size=fread (src_code, sizeof(char), 65536, src_file);
	char found='\0';
	for (int i=0;i<size;i++){
		char c=src_code[i];
		
		if (c=='\0' || c=='\n' || c==' ' || c=='\t'){
			continue;
		}
		if (c=='0' || c=='1'){
			if (found){
				syntax_error();
			} else {
				found=c;
			}
		} else {
			syntax_error();
		}
	}
	if (found!='0' && found!='1'){
			syntax_error();
	}

	int ends[2];
	pipe(ends);
	int id=fork();
	if (id==0){
		char * args[7];
		dup2(ends[0],0);
		close(ends[0]); close(ends[1]);
		args[0]="cc";
		args[1]="-xc";
		args[2]="-O3";
		if (argc==2){
			args[3]="-";
			args[4]=NULL;
		} else{
			args[3]="-o";
			args[4]=argv[2];
			args[5]="-";
			args[6]=NULL;
			execvp("cc",args);
		}
	} else {
		close(ends[0]);
		if(found=='0'){
			write(ends[1], falsey, strlen(falsey));
		} else { //if (found=='1')
			write(ends[1],truey, strlen(truey));
		}
		close(ends[1]);
		wait(NULL);
	}
	

	return 0;
}


