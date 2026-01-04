#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv){
	if (argc!=2){
		printf("Error: This interpreter takes exactly 1 argument, which must be a valid filepath to the bf source code.\n");
		exit(1);
	}

	FILE *source = fopen(argv[1], "rb");
	if (!source){
		printf("Error: Failed to open file.\n");
		exit(1);
	}
	fseek(source,0,SEEK_END);
	long codelen = ftell(source);
	rewind(source);

	char *code = malloc(codelen+1);
	if (!code){
		printf("Error: Failed to allocate memory.\n");
		fclose(source);
		exit(1);
	}

	fread(code,1,codelen,source);
	code[codelen] = '\0';
	fclose(source);

	int dataPointer = 0;
	int *data = NULL;
	data = malloc(30000*sizeof(int));
	int dataSize = 30000;
	if (!data){
		printf("Error: Failed to allocate memory.\n");
		free(code);
		exit(1);
	}
	for (int i=0;i<dataSize;i++){
		data[i]=0;
	}
	for (int i=0;i<codelen+1;i++){
		if (code[i]=='>'){
			dataPointer++;
			if (dataPointer>dataSize-1){
				int *tmp = realloc(data, dataSize*sizeof(int)+(100*sizeof(int)));
				if (!tmp){
					printf("Error: Failed to allocate memory.\n");
					free(code);
					free(data);
					exit(1);
				}
				data = tmp;
				dataSize+=100;
				for (int j=dataSize-100;j<dataSize;j++){
					data[j]=0;
				}
			}
		}
		else if (code[i]=='<'){
			dataPointer--;
			if (dataPointer<0){
				int *tmp = realloc(data, dataSize*sizeof(int)+(100*sizeof(int)));
				if (!tmp){
					printf("Error: Failed to allocate memory.\n");
					free(code);
					free(data);
					exit(1);
				}
				data = tmp;
				memmove(data+100,data,dataSize*sizeof(int));
				dataSize+=100;
				for (int j=0;j<100;j++){
					data[j]=0;
				}
				dataPointer+=100;
			}
		}
		else if (code[i]=='+'){
			data[dataPointer]++;
			if (data[dataPointer]>127){
				data[dataPointer]=0;
			}
		}
		else if (code[i]=='-'){
			data[dataPointer]--;
			if (data[dataPointer]<0){
				data[dataPointer]=127;
			}
		}
		else if (code[i]=='.'){
			printf("%c",data[dataPointer]);
		}
		else if (code[i]==','){
			char input = fgetc(stdin);
			if (input<0 || input>127){
				printf("Error: Invalid input.\n");
				free(code);
				free(data);
				exit(1);
			}
			data[dataPointer] = input;
		}
		else if (code[i]=='['){
			if (data[dataPointer]==0){
				int opencount = 1;
				while (code[i]!=']'||opencount!=0){
					i++;
					if (code[i]=='\0'){
						printf("Error: No matching ] found.\n");
						free(code);
						free(data);
						exit(1);
					}
					if (code[i]=='['){
						opencount++;
					}
					if (code[i]==']'){
						opencount--;
					}
				}
			}
		}
		else if (code[i]==']'){
			if (data[dataPointer]!=0){
				int closecount = 1;
				while (code[i]!='['||closecount!=0){
					i--;
					if (i<0){
						printf("Error: No matching [ found.\n");
						free(code);
						free(data);
						exit(1);
					}
					if (code[i]=='['){
						closecount--;
					}
					if (code[i]==']'){
						closecount++;
					}
				}
				i--;
			}
		}
	}
	
	free(code);
	free(data);
	exit(0);
}
