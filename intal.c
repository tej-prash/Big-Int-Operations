#include "intal.h"
#include <stdio.h>
#include <stdlib.h>
#include  <string.h> 
#include <math.h>
#include <stdbool.h>
typedef struct intal{
	long int *arr; //Although on Windows long int is 4 bytes(which is 
				   //the same size as an integer, this need not be true 
					// for other operating systems)
	long int digit_count;
	long int length;
}MyInt;
int how_many_digits(long long int sum);
MyInt * karatsuba(MyInt *,MyInt *);
void * mul_by_10(void *e,long int m);

void* intal_create(const char* str){
	MyInt * integer=(MyInt *)malloc(sizeof(MyInt)*1);
	integer->digit_count=strlen(str); //Using this to calculate the length 
								// of the string
	long int num_locations=ceil((integer->digit_count)/(float)9.0);
	integer->arr=(long int *)malloc(sizeof(long int)*(num_locations));
	integer->length=num_locations;
	if(integer->arr!=NULL){
		int j=0;long int portion=0;
		long int k=num_locations-1;
		for(long int i=integer->digit_count-1;i>=0;i--){
			if(j<=8){
				portion+=(str[i]-'0')*(int)pow(10,j);
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
		/*if(k==0)printf("Success\n");
		else{
			printf("%ld",k);
		}*/
		//printf("Increate %d\n",integer->arr[])
		return (void *)integer;
	}
	else{
		//exception handling
		printf("Failure\n");

	}
}
void intal_destroy(void* intal){
	MyInt *integer=(MyInt *)intal;
	free(integer->arr);
	free(intal);
}
char* intal2str(void* intal){
	MyInt *integer=(MyInt *)intal;
	char *string=(char *)malloc(sizeof(char)*(integer->digit_count+1));
	long int k=integer->digit_count-1;
	printf("String has %ld digits",integer->digit_count);
	string[k+1]='\0';
	for(long int i=integer->length-1;i>=0;i--){
		int j=0;
		long int temp=integer->arr[i];
		//printf("Array element i %ld",integer->arr[i]);
		if(integer->arr[i]==0){
			for(int j=0;j<9;j++){
				string[k]='0';
				k--;
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
void* intal_increment(void* intal){
	MyInt *integer=(MyInt *)intal;
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
		//printf("Digit count is %ld Sum %ld",integer->digit_count,sum);
		//printf("Sum and j:%ld %ld\n",sum,j);
	}
	return (void *)integer;
}
void* intal_decrement(void* intal){
	MyInt *integer=(MyInt *)intal;
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
void* intal_add(void* intal1, void* intal2){
	bool flag=1;
	//Typecasting
	MyInt *integer1=(MyInt *)intal1;
	MyInt *integer2=(MyInt *)intal2;
	
	//Creating the new intal to store the result
	MyInt *large,*small;
	if(integer1->length>=integer2->length){
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
	return (void *)result;
}
int how_many_digits(long int sum){
	int count=0;
	//printf("In how many digits: sum %ld",sum);
	sum/=
	while((long int)sum){
		sum=sum/10;
		count++;
	}
	return count;
}

void* intal_diff(void* intal1, void* intal2){
	int which=intal_compare((MyInt *)intal1,(MyInt *)intal2);
	MyInt *large,*small;
	MyInt *integer=(MyInt *)malloc(sizeof(MyInt));
	if(which==1){
		large=intal1;
		small=intal2;
	}
	else if(which==0){
		//return 0;
	}
	else{
		large=intal2;
		small=intal1;
	}		

	//Maximum length is that of the larger integer
	integer->length=large->length;

	//Maximum digit count is that of the larger integer
	integer->digit_count=large->digit_count;
	integer->arr=(long int *)malloc(sizeof(long int)*large->length);
	long int i=large->length-1;
	long int j=small->length-1;
	long int k=large->length-1;
	long int diff;
	int borrow=0;
	for((i+1)&&(j+1)){
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
	while(diff==0){
		long int *new_arr=(long int *)malloc(sizeof(long int)*(integer->length-1));
		for(int j=0;j<integer->length;j++){
				new_arr[j]=integer->arr[j+1];
		}
		free(integer->arr);
		integer->arr=new_arr;
		integer->length--;
		integer->digit_count=(integer->length*9);
		k++;
		diff=integer->arr[k];
	}
	int msb_count=how_many_digits(diff);
	integer->digit_count=msb_count + (integer->length-1)*(9)
	return integer;

}
int intal_compare(void* intal1, void* intal2){
	intal1=(MyInt *)intal1;
	intal2=(MyInt *)intal2;
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
void* intal_multiply(void* intal1, void* intal2){
	static int calls=0;
	static MyInt *result=NULL;
	intal1=(MyInt *)intal1;
	intal2=(MyInt *)intal2;
	MyInt *large,*small;
	if(calls==0){
		if(result==NULL)result=(MyInt *)calloc(sizeof(MyInt));
		long int num_locations=ceil(intal1->length+intal2->length);
		result->arr=(long int *)calloc(sizeof(long int)*(num_locations));
		result->length=num_locations;
		result->digit_count=intal1->digit_count+intal2->digit_count;
	}	
	calls++;
	long int n,m; //n is always larger than  m
	if(intal1->length>intal2->length{
			large=intal1;
			small=intal2;
			
	}
	else if(intal1->length<intal2->length){
			large=intal2;
			small=intal1;
	}
	else{
		calls--;
		return karatsuba((void *)intal1,(void *)intal2,(void *)result);
	}
	n=large->length;
	m=small->length;

	//Larger number
	MyInt int1=*large;
	int1.arr=large->arr;
	int1.length=n-m;
	int1.digit_count=large->digit_count-(m*9)

	//Smaller number
	MyInt int2=*small;
	int2.arr=small->arr;
	int2.length=m;
	int2.digit_count=small->digit_count;

	//Computing e(until e has a length of m)
	//Returns a new structure
	MyInt *e=intal_multiply((void *)large,(void *small));

	//Keeps track of recursion depth
	calls--;

	int1=*large;
	int1.arr=large->arr +(n-m);
	int1.length=m;
	int1.digit_count=m*9;

	//Smaller number
	int1=*small;
	int2.arr=small->arr;
	int2.length=m;
	int2.digit_count=small->digit_count;

	//Multiply e by 10^m,inplace operation

	e=mul_by_10(e,m);
	//
	MyInt *f=karatsuba(int1,int2);

	MyInt *partial_sum=intal_add((void *)e,(void *)f);
	result=intal_add((void *)result,(void *partial_sum));
	free(e);
	free(f);
	free(partial_sum);
	if(calls==0){
		result=NULL;
	}
	return result;
}
void * mul_by_10(void *e,long int m){
	MyInt *int1=e;
	long int num_locations=ceil(m/(float)9.0);
	int1->length+=num_locations;
	int1->digit_count+=m;
	int1=(MyInt *)realloc(int1->arr,sizeof(long int)*(int1->length));
	for(int i=int1->length-1;i>=int1->length-num_locations;i--){
			int1->arr[i]=0;
	}
	return (void *)int1;
}
MyInt * karatsuba((MyInt *)in1,(My Int*)in2){
	/*static int karat_calls=0;
	static MyInt *result_karat=NULL;
	if(calls==0){
		if(result_karat==NULL)result_karat=(MyInt *)calloc(sizeof(MyInt));
		long int num_locations=ceil(intal1->length+intal2->length);
		result_karat->arr=(long int *)calloc(sizeof(long int)*(num_locations));
		result_karat->length=num_locations;
		result_karat->digit_count=intal1->digit_count+intal2->digit_count;
	}	
	karat_calls++;*/
	if(in1->length==1 && in2->length==1){
		long long int res=in1->arr[0] * in2->arr[0];
		MyInt *integer=(MyInt *)malloc(sizeof(MyInt));
		integer->length=1;
		integer->digit_count=how_many_digits(res);
		integer->arr=(long int *)malloc(sizeof(long int));
		if(integer->digit_count>9){
			integer->length++;
			integer->arr=(long int *)realloc(sizeof(long int)*2);
			char *str_integer=(char *)malloc(sizeof(char)*integer->digit_count);
			lltoa(res,str_integer,10);
			integer->arr[1]=itoa(str_integer+integer->digit_count-9)
			str_integer[len(str_integer)-9]='\0'
			integer->arr[0]=itoa(str_integer);
			free(str_integer);
		}
		else{
			integer->arr[0]=res;
		}
		//karat_calls--;
		return integer;
	}
	else{
		if(in1->length==in2->length){
			long int n=intal1->length;
			long int m=n/2;
			long int *intal1,*intal2;
			bool odd=0;
			if(n%2){
				odd=1;
				intal1=(long int *)malloc(sizeof(long int)*in1->length+1);
				for(int i=1;i<in1->length;i++){
					intal1[i]=in1[i];
				}
				intal1->length=in1->length+1;
				intal1->digit_count=in1->digit_count+1;
				intal1[0]=0;

				intal2=(long int *)malloc(sizeof(long int)*in2->length+1);
				for(int i=1;i<in2->length;i++){
					intal2[i]=in2[i];
				}
				intal2[0]=0;
				intal2->length=in2->length+1;
				intal2->digit_count=in2->digit_count+1;
				n++;
				m=n/2;
			}
			else{
				intal1=in1;
				intal2=in2;
			}
			//Numbers are divided into a1a0
									// b1b0
			//a1
			MyInt a1=*intal1;
			a1.arr=intal1->arr;
			a1.length=m;
			if(n!=1)a1.digit_count=intal1->digit_count-((n-m)*9);
			else a1.digit_count=intal1->digit_count;
			//b1
			MyInt b1=*intal2;
			b1.arr=intal2->arr;
			b1.length=m;
			if(n!=1)b1.digit_count=intal2->digit_count-((n-m)*9);
			else b1.digit_count=intal2->digit_count;

			MyInt *p1=karatsuba(&a1,&b1); //(a1,b1)

			//a0
			MyInt a0=*intal1;
			if(n!=1)a0.arr=intal1->arr + n-m;
			else a0.arr=intal1->arr;
			a0.length=n-m;
			if(n!=1)a0.digit_count=(n-m)*9;
			else a0.digit_count=intal1->digit_count;

			//b0
			MyInt b0=*intal2;
			if(n!=1)b0.arr=intal2->arr + n-m;
			else b0.arr=intal2->arr;
			b0.length=n-m;
			if(n!=1)b0.digit_count=(n-m)*9;
			else b0.digit_count=intal2->digit_count;

			MyInt *p2=karatsuba(&a0,&b0); //(a0,b0)

			MyInt *a=intal_add(&a1,&a0);
			MyInt *b=intal_add(&b1,&b0);

			MyInt *p3=intal_multiply(a,b);

			if(n%2){p1=mul_by_10(p1,n+1)};
			else{p1=mul_by_10(p1,n);}

			MyInt *first_sub=intal_diff(p3,p1)

			MyInt *second_sub=intal_diff(first_sub,p2);

			if(m%2){second_sub=mul_by_10(second_sub,m+1);}
			else{second_sub=mul_by_10(second_sub,m);}

			MyInt *first_sum=intal_add(p1,second_sub);

			MyInt *result=intal_add(first_sum,p2);

			intal_destroy((void *)p1);
			intal_destroy((void *)p2);
			intal_destroy((void *)p3);
			intal_destroy((void *)first_sub);
			intal_destroy((void *)second_sub);
			intal_destroy((void *)first_sum);
			if(odd){
				intal_destroy((void *)intal1);
				intal_destroy((void *)intal2);
			}

			return result;
			//Keep track of recursion depth	
			//karat_calls--;
		}
		else{
			printf("Failure. n doesn't match m")
		}	
	}
}
void* intal_divide(void* intal1, void* intal2){

}
