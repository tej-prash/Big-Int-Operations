#include "intal.h"
#include <stdio.h>
#include <stdlib.h>
#include  <string.h>  
/*
The following implementation can also be done:
typedef struct intal{
	int *arr;
	int length;
}MyInt;
In this way, each element of the array "arr" can store 10 digits and since malloc 
can be used to allocate about 10^9 memory locations, an integer with 
10^10 digits 
*/
typedef struct intal{
	char *arr;
	int length;
}MyInt;
void* intal_create(const char* str){
		/*MyInt *integer=(MyInt *)malloc(sizeof(MyInt)*1);
		integer->arr=(long long int *)malloc(sizeof(long long int)*2*100000000);
		integer->length=0;
		if(integer->arr!=NULL)printf("Success\n");
		else printf("Failure\n");
		return (void *)integer;*/
		MyInt * integer=(MyInt *)malloc(sizeof(MyInt)*1);
		integer->length=strlen(str); //Using this to calculate the length 
									// of the string
		integer->arr=(char *)malloc(sizeof(char)*(integer->length));
		if(integer->arr!=NULL){
			for(int i=0;i<integer->length;i++){
				integer->arr[i]=str[i];
			}
			return (void *)integer;
		}
	else{
		//exception handling

	}
}
void intal_destroy(void* intal){
	free(intal);
}

char* intal2str(void* intal){
	return ((MyInt *)intal)->arr;
}

void* intal_increment(void* intal){
	MyInt *integer=(MyInt *)intal;
	int sum=(integer->arr)[integer->length-1]-'0'+1;
	int j=integer->length-1;
	while((int)sum/10 && j){
		(integer->arr)[j]=(sum%10)+'0';
		j--;
		sum=(sum/10)+(integer->arr)[j]-'0';
	}	//i love teaaaaaaa
	if(j==0){
		if((int)(sum/10)){
				char *new_arr=(char *)malloc(sizeof(char)*(integer->length+1));
				strcpy(new_arr+1,(integer->arr));
				new_arr[1]=(sum%10)+'0';
				new_arr[0]=(sum/10)+'0';
				free(integer->arr);
				integer->arr=new_arr;
		}
		else{
			(integer->arr)[j]=(sum%10)+'0';
		}		
	}
	if(j==integer->length-1){
		(integer->arr)[integer->length-1]=(integer->arr)[integer->length-1]+'0';
	}
	return (void *)integer->arr;
}