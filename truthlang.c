#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
void syntax_error(){
	fprintf(stderr,"Syntax Error!\n");
	exit(1);
}
char * falsey="#include <stdio.h>\nint main(void){printf(\"0\\n\");return 0;}\n";
char * truey="#include <stdio.h>\nint main(void){while(1)printf(\"1\");}\n";
char * tmp_filename="/tmp/a.c";
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
	FILE * tmp = fopen(tmp_filename,"w");
	char * args[6];
	
	args[0]="cc";
	if(found=='0'){
		fputs(falsey, tmp);
	} else if (found=='1'){
		fputs(truey,tmp);
	} else {
		syntax_error();
	}
	fclose(tmp);
	if (argc==2){
		args[1]=tmp_filename;
		args[2]="-O3";
		args[3]=NULL;
	} else{
		args[1]="-o";
		args[2]=argv[2];
		args[3]=tmp_filename;
		args[4]="-O3";
		args[5]=NULL;
	}
	
	execvp("cc",args);
	return 0;
}


