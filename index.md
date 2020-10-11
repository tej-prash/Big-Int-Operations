## Introduction

The library provides an efficient implementation of basic arithmetic operations with integers of arbitrary length. 

## Description

The C-based library uses the following data structure to represent an integer: 
```
typedef struct intal{
	long int *arr; //An array of long integers
	long int digit_count; //Total number of digits in the integer
	long int length; //Length of long integer array
}MyInt;
```

The structure efficiently utilizes space by using an internal array representation of long integers. Each element of the array consists of a series of digits occupying as many bytes as supported by a long integer in C. A series of array elements together can represent an integer of arbitrary length. 

When provided with a string-based input of an integer, it is internally represented using the above structure and various arithmetic operations can be performed using it. The arithmetic operations that are supported are:
- Increment
- Decrement
- Addition/Subtraction
- Multiplication/Division
- Exponentiation
- Comparison



## Results
### Sample Input 
Consider the following two integers provided as input:
1. Integer 1:15453844568042674261155006622975933139565572846562378528540325003787716869493326221313083641348849
2. Integer 2: 3931137821044013493791098177239438243262656520793

### Sample Output

<img src="https://user-images.githubusercontent.com/31772714/95673417-eb5aee80-0bc5-11eb-9b10-751f3cf7cfec.png" width="800" height="250">
