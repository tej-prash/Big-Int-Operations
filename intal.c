#include "intal.h"
#include <stdio.h>
#include <stdlib.h>
#include  <string.h> 
#include <math.h>
#include <stdbool.h>
#include <ctype.h>
typedef struct intal{
	long int *arr; //Although on Windows long int is 4 bytes(which is 
				   //the same size as an integer, this need not be true 
					// for other operating systems)
	long int digit_count;
	long int length;
}MyInt;
static int how_many_digits(long long int sum);
static void * normal_mult(void *int1,void *int2);
static void * mul_by_10(MyInt *e,long int m);

void* intal_create(const char* str){
	if(str==NULL)return NULL;
	MyInt * integer=(MyInt *)malloc(sizeof(MyInt)*1);
	//char *str=str1;
	int flag=1;
	int len=strlen(str);
	char *string=(char *)malloc(sizeof(char)*(len+1));
	strcpy(string,str);
	int offset=0;//keeps track of base address
	for(int i=0;i<len;i++){
		if(isdigit(*string)){
			if((*string)=='0'){
				string++;offset++;
			}	
			else break;
		}
		else{
			break;
		}
	}
	int count_dig=0;
	while(flag && count_dig<strlen(string)){
		if(!isdigit(string[count_dig])){
			flag=0;
		}
		else{
			//printf("%d\n",count_dig);
			count_dig++;
		}
	}
	if(count_dig<strlen(string)){
		string[count_dig]='\0';
	}	
	if(*string=='\0'){
		integer->digit_count=1;
		integer->length=1;
		integer->arr=(long int *)malloc(sizeof(long int));
		integer->arr[0]=0;
		string=string-offset;
		free(string);
		return (void *)integer;
	}

	integer->digit_count=strlen(string); //Using this to calculate the length 
								// of the string
	long int num_locations=ceil((integer->digit_count)/(float)9.0);
	integer->arr=(long int *)malloc(sizeof(long int)*(num_locations));
	integer->length=num_locations;
	if(integer->arr!=NULL){
		int j=0;long int portion=0;
		long int k=num_locations-1;
		for(long int i=integer->digit_count-1;i>=0;i--){
			if(j<=8){
				portion+=(string[i]-'0')*(int)pow(10,j);
				j++;
				if(j==9){
					integer->arr[k]=portion;
					//printf("Portion:%d\n",portion);
					portion=0;
					j=0;
					k--;
				}	
			}
		}
		if(j){
			integer->arr[k]=portion;
			//printf("Portion beg %d:",portion);
		}
		string=string-offset;
		free(string);
		return (void *)integer;
	}


}
void intal_destroy(void* intal){
	if(intal!=NULL){
		MyInt *integer=(MyInt *)intal;
		///printf("Entered  destroy\n");
		free(integer->arr);
		//printf("Array is successfully deallocated\n");
		free(integer);
		//printf("intal object is deallocated\n");
	}
}
char* intal2str(void* intal){
	MyInt *integer=(MyInt *)intal;
	if(intal!=NULL){
		char *string=(char *)malloc(sizeof(char)*(integer->digit_count+1));
		long int k=integer->digit_count-1;
		//printf("String has %ld digits length %ld\n",integer->digit_count,integer->length);
		string[k+1]='\0';
		for(long int i=integer->length-1;i>=0;i--){
			int j=0;
			long int temp=integer->arr[i];
			//printf("Array element i %ld",integer->arr[i]);
			if(integer->arr[i]==0){
				int j=0;
				while(j<9 && k>=0){
					string[k]='0';
					k--;
					j++;
				}
				continue;	
			}
			while(((int)(temp)||j<9) && k>=0){
				string[k]=(temp%10)+'0';
				//printf("in string:%c\n",string[k]);
				temp=temp/10;
				k--;
				j++;
			}
		}
		return string;
	}
	else{
		char *string=(char *)malloc(sizeof(char)*4);
		strcpy(string,"NaN");
		return string;
	}
}
void* intal_increment(void* intal){
	MyInt *integer=(MyInt *)intal;
	if(intal!=NULL){
		long int sum=integer->arr[integer->length-1]+1;
		long int j=integer->length-1;
		int flag=1;
		while(((int)sum/(long int)pow(10,9))&&flag){
			//Extracting most significant digit and store it in the variable carry
			/*int *temp=&sum;
			//If big endian
			unsigned int i=1;
			char *c=(char *)&i;
			if(*c){
				//If little endian
				int carry=*(temp+4);
			}
			else{	
				//If big endian
				int carry=*temp;
			}	*/
			//int carry=1; //Carry is always 1 if there is an overflow
			int carry=sum/(long int)pow(10,9);
			integer->arr[j]=sum-(long int)(carry*(long int)pow(10,9));
			j--;
			if(j!=-1) sum=integer->arr[j]+carry;
			else{
				long int *new_arr=(long int*)malloc(sizeof(integer->length)+1);
				for(int i=0;i<integer->length;i++){
					new_arr[i+1]=integer->arr[i];
				}
				free(integer->arr);
				new_arr[0]=carry;
				integer->arr=new_arr;
				integer->length++;
				integer->digit_count++;
				flag=0;
			}
			//printf("%ld",integer->arr[j]);
		}
		if(flag && j==0){
			int msb_dig_count=how_many_digits(sum);
			integer->digit_count=msb_dig_count+(integer->length-1)*(9);
			integer->arr[j]=sum;
			return (void *)integer;
			//printf("Digit count is %ld Sum %ld",integer->digit_count,sum);
			//printf("Sum and j:%ld %ld\n",sum,j);
		}
		integer->arr[j]=sum;
		//printf("Last element %ld\n",integer->arr[integer->length-1]);
		return (void *)integer;
	}
	else return NULL;
}
void* intal_decrement(void* intal){
	MyInt *integer=(MyInt *)intal;
	if(integer!=NULL){
		if(integer->digit_count==1&&integer->arr[0]==0){
			return (void *)integer;
		}
		long int j=0;
		bool flag=0;
		while(integer->arr[integer->length-j-1]<1){
			integer->arr[integer->length-j-1]=999999999; //If the last element is zero
			j++;
			flag=1;
		}
		integer->arr[integer->length-j-1]-=1;
		if(integer->arr[integer->length-j-1]==0){ //if result is zero, decrease size of array
			long int *new_arr=(long int *)malloc(sizeof(integer->length)-1);
			//memcpy(new_arr,(integer->arr)+1,integer->length-1);
			for(int i=0;i<integer->length-1;i++){
				new_arr[i]=integer->arr[i+1];
			}
			free(integer->arr);
			integer->arr=new_arr;
			integer->digit_count--;
			integer->length--;
			//printf("Should be 8 nines:%d\n",new_arr[0]);
		}
		else{
			//Check if the decrement resulted in an integer equal to "9"*i (1<=i<=9)
			//If it did, decrement the number of digits by one
			long int start=9;
			bool nine_or_not=0;
			for(int i=1;i<9;i++){
				if(integer->arr[integer->length-j-1]==start){
					nine_or_not=1;
					break;
				}
				start+=9*(int)pow(10,i);
			}
			if(nine_or_not){
				integer->digit_count--;
			}
		}
		//printf("In decrement:%d %d\n",integer->arr[0],integer->arr[1]);
		return (void *)integer;
	}
	else return NULL;
}
void* intal_add(void* intal1, void* intal2){
	bool flag=1;
	//Typecasting
	MyInt *integer1=(MyInt *)intal1;
	MyInt *integer2=(MyInt *)intal2;
	
	if(intal1!=NULL && intal2!=NULL){
		//Creating the new intal to store the result
		MyInt *large,*small;
		if(integer1->digit_count>=integer2->digit_count){
			large=integer1;
			small=integer2;
		}
		else{
			large=integer2;
			small=integer1;
		}
		MyInt *result=(MyInt *)malloc(sizeof(MyInt));
		result->digit_count=large->digit_count;
		long int num_locations=ceil((result->digit_count)/(float)9.0);
		result->arr=(long int *)malloc(sizeof(long int)*(num_locations));
		result->length=num_locations;
		//printf("In add result length%ld result digitcount%ld large digitcount %ld\n",result->length,result->digit_count,large->digit_count);
		//Adding the 2 intals
		long int i=large->length-1,j=small->length-1;
		int carry=0;
		long int k=large->length-1;
		long int sum;
		while((i+1) && (j+1)){
			sum=(large->arr)[i]+(small->arr)[j]+carry;
			//The maximum value of the resultant sum obtained is of the order 10^10 
			carry=sum/(long int)pow(10,9);
			result->arr[k]=sum-(long int)(carry*(long int)pow(10,9));
			k--;
			i--;j--;
			//printf("%ld sum %ld carry %d",result->arr[0],sum,carry);
		}
		while(i+1){ //Integer1 is larger
			sum=large->arr[i]+carry;
			//Add carry only to the larger integer
			carry=sum/(long int)pow(10,9);
			if(carry)result->arr[k]=sum-(long int)(carry*(long int)pow(10,9));
			else result->arr[k]=sum;
			k--;i--;		
		}
		if(carry){
			//printf("Reallocating memory\n");
			//printf("Carry:%d\n",carry);
			long int *new_arr=(long int*)malloc(sizeof(result->length)+1);
			for(int i=0;i<result->length;i++){
				new_arr[i+1]=result->arr[i];
				//printf("When copying %ld",result->arr[i]);
			}
			free(result->arr);
			result->arr=new_arr;
			new_arr[0]=carry;
			result->arr=new_arr;
			result->length++;
			result->digit_count++;
			flag=0;
		}
		if(flag && i==-1){
			//printf("In add sum %ld",sum);
			int msb_dig_count=how_many_digits(sum);
			result->digit_count=msb_dig_count+(result->length-1)*(9);
		}
		//printf("returned from add\n");
		return (void *)result;
	}
	else return NULL;
}
static int how_many_digits(long long int sum){
	int count=0;
	if(sum==0)return 1;
	//printf("In how many digits: sum %ld",sum);
	while((long long int)sum){
		sum=sum/10;
		count++;
	}
	return count;
}

void* intal_diff(void* intal1, void* intal2){
	int which=intal_compare((void *)intal1,(void *)intal2);
	if((MyInt *)intal1!=NULL && (MyInt *)intal2!=NULL){		
		MyInt *large,*small;
		MyInt *integer=(MyInt *)malloc(sizeof(MyInt));
		if(which==1){
			large=intal1;
			small=intal2;
		}
		else if(which==0){
			integer->digit_count=1;
			integer->length=1;
			integer->arr=(long int *)malloc(sizeof(long int)*1);
			integer->arr[0]=0;
			return integer;
		}
		else{
			large=intal2;
			small=intal1;
		}		

		//Maximum length is that of the larger integer
		integer->length=large->length;

		//Maximum digit count is that of the larger integer
		integer->digit_count=large->digit_count;
		//printf("large length %ld",large->length);
		integer->arr=(long int *)calloc(large->length,sizeof(long int));
		long int i=large->length-1;
		long int j=small->length-1;
		long int k=large->length-1;
		long int diff;
		int borrow=0;
		while((i+1)&&(j+1)){
			diff=(large->arr)[i]-(small->arr)[j]+borrow;
			if(diff<0){
				integer->arr[k]=diff+(int)pow(10,9);
				borrow=-1;
			}
			else{
				integer->arr[k]=diff;
				borrow=0;
			}
			k--;
			i--;j--;
		}
		while(i+1){
			diff=(large->arr)[i]+borrow;
			if(diff<0){
				integer->arr[k]=diff+(int)pow(10,9);
				borrow=-1;
			}
			else{
				integer->arr[k]=diff;
				borrow=0;
			}
			k--;
			i--;
		}
		k=0;
		while(diff==0 && integer->length!=1){
			long int *new_arr=(long int *)malloc(sizeof(long int)*(integer->length-1));
			for(int j=1;j<integer->length;j++){
					new_arr[j-1]=integer->arr[j];
			}
			free(integer->arr);
			integer->arr=new_arr;
			integer->length--;
			integer->digit_count=(integer->length*9);
			//if(k>=integer->length)break;
			diff=integer->arr[k];
		}
		int msb_count=how_many_digits(diff);
		integer->digit_count=msb_count + (integer->length-1)*(9);
		return integer;
	}
	else return NULL;

}
int intal_compare(void* in1, void* in2){
	//printf("Entered compare\n");
	MyInt *intal1=(MyInt *)in1;
	MyInt *intal2=(MyInt *)in2;
	if(intal1==NULL || intal2==NULL)return -2;
	//printf("sdfs%ld %ld",intal1->length,intal2->length);
	if(intal1->digit_count>intal2->digit_count)return 1;
	else if(intal1->digit_count<intal2->digit_count)return -1;
	else{
		for(int i=0;i<intal1->length;i++){
			if(intal1->arr[i]>intal2->arr[i])return 1;
			else if(intal1->arr[i]<intal2->arr[i]) return -1;
		}
		return 0;
	}
}
void* intal_multiply(void* int1, void* int2){
	/*static int calls=0;
	static MyInt *result=NULL;*/
	MyInt *in1=(MyInt *)int1;
	MyInt *in2=(MyInt *)int2;
	if(in1!=NULL && in2!=NULL){
		MyInt *intal1=(MyInt *)malloc(sizeof(MyInt));
		intal1->arr=(long int *)malloc(sizeof(long int)*in1->length);
		intal1->length=in1->length;
		intal1->digit_count=in1->digit_count;
		for(int i=0;i<intal1->length;i++){
			intal1->arr[i]=in1->arr[i];
		}

		MyInt *intal2=(MyInt *)malloc(sizeof(MyInt));
		intal2->arr=(long int *)malloc(sizeof(long int)*in2->length);
		intal2->length=in2->length;
		intal2->digit_count=in2->digit_count;
		for(int i=0;i<intal2->length;i++){
			intal2->arr[i]=in2->arr[i];
		}
		MyInt *result=normal_mult((void *)intal1,(void *)intal2);
		intal_destroy((void *)intal1);
		intal_destroy((void *)intal2);

		long int diff=result->arr[0];
		//printf("%ld",diff);
		while(diff==0 && result->length!=1){
				long int *new_arr=(long int *)malloc(sizeof(long int)*(result->length-1));
				for(int j=1;j<result->length;j++){
						new_arr[j-1]=result->arr[j];
				}
				free(result->arr);
				result->arr=new_arr;
				result->length--;
				result->digit_count=(result->length*9);
				diff=result->arr[0];
		}
		int msb_count=how_many_digits(diff);
		result->digit_count=msb_count + (result->length-1)*(9);
		return result;	
	}
	else return NULL;
}
static void * normal_mult(void *int1,void *int2){
	MyInt *intal1=(MyInt *)int1;
	MyInt *intal2=(MyInt *)int2;
	MyInt *large,*small;
	long int n,m;
	if(intal1->length>=intal2->length){
		large=intal1;
		small=intal2;
	}
	else{
		large=intal2;
		small=intal1;
	}
	n=large->length;
	m=small->length;
	MyInt *result=(MyInt *)malloc(sizeof(MyInt));
	result->arr=(long int *)calloc(1,sizeof(long int));
	result->length=1;
	result->digit_count=1;
	long int i=small->length-1;
	long int j=large->length-1;
	MyInt *partial;
	while(i+1){
		partial=(MyInt *)malloc(sizeof(MyInt));
		partial->arr=(long int *)calloc(n,sizeof(long int));
		partial->length=n;
		partial->digit_count=0;
		//printf("%pAt the beginning :result has %ld length %ld digits\n",result,result->length,result->digit_count);
		//partial->digit_count=(n+1)*9;
		j=large->length-1;
		long int k=partial->length-1;
		int carry=0;
		bool flag=0;
		while(j+1){
			long long int res=small->arr[i]*large->arr[j];
			int dig_count=how_many_digits(res);
			/*if(res==0)partial->digit_count+=9;
			else partial->digit_count+=dig_count;*/
			//printf("In between :result has %ld length %ld digits\n",result->length,result->digit_count);
			if(dig_count>9){
				char *str_integer=(char *)malloc(sizeof(char)*dig_count);
				sprintf(str_integer,"%lld",res);
				//printf("str_integer%s\n",str_integer);
				long int sum=atoi(str_integer+dig_count-9)+partial->arr[k]+carry;
				//printf("exceeding 9? %d\n",how_many_digits(sum));
				carry=sum/(int)pow(10,9);
				partial->arr[k]=sum-(long int)(carry*(long int)pow(10,9));
				str_integer[dig_count-9]='\0';
				if(k==0){
					long int *new_arr=(long int *)malloc(sizeof(long int)*(partial->length+1));
					for(int i=partial->length-1;i>=0;i--){
						new_arr[i+1]=partial->arr[i];
					}
					long int sum=atoi(str_integer);
					new_arr[0]=sum;
					partial->length++;
					free(partial->arr);
					partial->arr=new_arr;
					//flag=1;
				}
				else{
					partial->arr[k-1]=atoi(str_integer);
				}
				free(str_integer);
			}
			else{
				long int sum=partial->arr[k]+res;
				carry=sum/(int)pow(10,9);
				partial->arr[k]=sum-(long int)(carry*(long int)pow(10,9));
				//partial->arr[k]+=res;
			}
			k--;
			j--;
		}
		if(carry){	
			long int sum=carry+partial->arr[0];
			carry=sum/(int)pow(10,9);
			partial->arr[0]=sum-(long int)(carry*(long int)pow(10,9));
			if(carry){
				long int *new_arr=(long int *)malloc(sizeof(long int)*(partial->length+1));
				for(int i=0;i<partial->length;i++){
					new_arr[i+1]=partial->arr[i];
				}
				new_arr[0]=carry;
				partial->length++;
				free(partial->arr);
				partial->arr=new_arr;
			}
		}
		int msb_count=how_many_digits(partial->arr[0]);
		partial->digit_count=(msb_count)+(partial->length-1)*9;
		//printf("partial %s\n",intal2str(partial));
		//printf("partial length and digit count %ld %ld\n",partial->length,partial->digit_count);
		partial=mul_by_10(partial,small->length-1-i);
		//printf("partial length and digit count %ld %ld\n",partial->length,partial->digit_count);
		//printf("result length and digit count %ld %ld\n",result->length,result->digit_count);
		MyInt *new_result=intal_add((void *)partial,(void *)result);
		intal_destroy((void *)result);
		result=new_result;
		//printf("%p After assignment result length nad digit count %ld %ld\n",result,result->length,result->digit_count);
		i--;
		//printf("Value of i is %ld\n",i);
		intal_destroy((void *)partial);
	}
	//printf("Returning from normal_mult\n");
	return result;
}
static void * mul_by_10(MyInt *e,long int m){
	//printf("Entered mul by 10 m is %ld\n",m);
	if(e->arr[0]==0 && e->length==1) return e;
	long int num_locations=m;
	MyInt *int1=(MyInt *)malloc(sizeof(MyInt));
	int1->length=e->length+num_locations;
	int1->digit_count=e->digit_count+(m*9);
	//printf("num locations initial length%ld %ld",num_locations,int1->length);
	//MyInt *int1=(MyInt *)malloc(sizeof(MyInt));
	
	//int1->length+=num_locations;
	//int1->digit_count+=m;
	//free(e);
	int1->arr=(long int *)calloc((int1->length),sizeof(long int));
	for(int i=0;i<e->length;i++){
		int1->arr[i]=e->arr[i];
	}
	//printf("Realloc done length digitcount %ld %ld\n",int1->length,int1->digit_count);
	/*for(int i=int1->length-1;i>=int1->length-num_locations;i--){
			int1->arr[i]=0;
	}*/
	//printf("in mul by 10 %ld %ld",int1->length,int1->arr[0]);
	//intal_destroy((void *)e);
	return (void *)int1;
}
void* intal_divide(void* in1, void* in2){
	MyInt *int1=(MyInt *)in1; //dividend
	MyInt *int2=(MyInt *)in2; //divisor
	if(int1==NULL || int2==NULL) return NULL;
	if(int2->digit_count==1&&int2->arr[0]==0)return NULL;
	if(int2->length==1 && int2->arr[0]==0)return NULL;
	char *quotient;
	MyInt *result;
	char *dividend=intal2str(in1);
	if(int1->digit_count<int2->digit_count){
		quotient=(char *)malloc(sizeof(char));
		quotient[0]='0';	
		result=intal_create(quotient);
		free(quotient);
		return result;
	}
	else{
		MyInt *divisor=int2;
		quotient=(char *)malloc(sizeof(char) * (int1->digit_count +1));
		char *dividend=intal2str(int1);
		char *temp=(char *)malloc(sizeof(char) * (int1->digit_count+1));
		strcpy(temp,dividend);
		long int num_digits_traversed=divisor->digit_count;
		temp[num_digits_traversed]='\0';
		MyInt *intal_dividend;
		long int k=0;
		bool flag=1;
		while(flag){
			int count=0;
			intal_dividend=intal_create((const char *)temp);
			//printf("temp is %s\n",temp);
			while(intal_compare((void *)intal_dividend,(void *)divisor)>=0){
				count++;
				//printf("%s\n",intal2str((void *)intal_dividend));
				MyInt *diff=intal_diff((void *)intal_dividend,(void *)divisor);
				char *temp1=intal2str(diff);
				free(temp);
				temp=temp1;
				intal_destroy((void *)diff);
				//intal_destroy((void *)intal_dividend);		
				intal_dividend=(MyInt *)intal_create((const char *)temp);
			}
			quotient[k]=count+'0';	
			//if(count==0)printf("%s\n",intal2str((void *)intal_dividend));
			//printf("quotient[k] %ld %c\n",k,quotient[k]);
			intal_destroy((void *)intal_dividend);			
			k++;
			int len=strlen(temp);
			if(dividend[num_digits_traversed]=='\0')flag=0;
			else {
				temp=(char *)realloc(temp,sizeof(char)*(len+2));
				temp[len]=dividend[num_digits_traversed];
				temp[len+1]='\0';
				num_digits_traversed++;
			}
		}	
		quotient[k]='\0';
		result=intal_create((const char *)quotient);	
		free(quotient);
		return result;
	}
}

int compare_with_1(void * intl){
	MyInt *intal=(MyInt *)intl;
	if(intal->digit_count==1 && intal->arr[0]==1)return 1;
	else return 0;
}
int is_odd(void * intal){
	MyInt *int1=(MyInt *)intal;
	if(((int1->arr[int1->length-1])%10)%2){
		return 1;
	}
	else return 0;
}
void* intal_pow(void* int1, void* int2){
	MyInt *intal1=(MyInt *)int1; //base
	MyInt *intal2=(MyInt *)int2;//exponent
	if(intal1==NULL||intal2==NULL)return NULL;
	if(intal1->digit_count==1 && intal1->arr[0]==0){ //if base is 0
		MyInt *result=(MyInt *)malloc(sizeof(MyInt));
		result->arr=(long int *)malloc(sizeof(long int));
		result->arr[0]=0;
		result->length=1;
		result->digit_count=1;
		return result;
	}	
	if(intal2->digit_count==1 && intal2->arr[0]==0){ //if exponent is 0
		MyInt *result=(MyInt *)malloc(sizeof(MyInt));
		result->arr=(long int *)malloc(sizeof(long int));
		result->arr[0]=1;
		result->length=1;
		result->digit_count=1;
		return result;
	}

	if(compare_with_1((void *)intal2)){ //base condition: if n becomes 1, return a
		MyInt *result1=(MyInt *)malloc(sizeof(MyInt));
		result1->length=intal1->length;
		result1->digit_count=intal1->digit_count;
		result1->arr=(long int *)malloc(sizeof(long int)*intal1->length);
		for(int i=0;i<intal1->length;i++){
			result1->arr[i]=intal1->arr[i];
		}
		return result1;
	}
	static MyInt *number_2=NULL;
	if(number_2==NULL){
		number_2=(MyInt *)malloc(sizeof(MyInt));
		number_2->arr=(long int *)malloc(sizeof(long int));
		number_2->arr[0]=2;
		number_2->length=1;
		number_2->digit_count=1;
	}
	
	MyInt *n_by_2=intal_divide((void *)intal2,(void *)number_2);	//calculating n/2
	
	MyInt *k=intal_pow((void *)intal1,(void *)n_by_2); //calculate pow(a,n/2)
	intal_destroy((void *)n_by_2);

	MyInt *k_copy=(MyInt *)malloc(sizeof(MyInt));
	k_copy->length=k->length;
	k_copy->digit_count=k->digit_count;
	k_copy->arr=(long int *)malloc(sizeof(long int)*k->length);
	for(int i=0;i<k->length;i++){
		k_copy->arr[i]=k->arr[i];
	}
	MyInt *result=intal_multiply((void *)k,(void *)k_copy); //multiply pow(a,n/2)with pow(a,n/2)
	//printf("returned from intal_multiply %p\n",result);
	if(is_odd((void *)intal2)){ //check if n is odd
		MyInt *result_final=intal_multiply((void *)result,(void *)intal1);
		//printf("In expo\n");
		intal_destroy((void *)k);
		intal_destroy((void *)k_copy);
		intal_destroy((void *)result);
		return result_final;
	}
	else{
		intal_destroy((void *)k);
		intal_destroy((void *)k_copy);
		return result;
	}
}