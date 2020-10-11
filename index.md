## Introduction

The library provides an efficient implementation of basic arithmetic operations with integers of arbitrary length. 

## Design

The C-based library uses the following data structure to represent an integer: 
```
typedef struct intal{
	long int *arr; //An array of long integers
	long int digit_count; //Total number of digits in the integer
	long int length; //Length of long integer array
}MyInt;
```

When provided with a string-based input of an integer, it is internally represented using the above structure and various arithmetic operations can be performed using it. The arithmetic operations that are supported are:
- Increment
- Decrement
- Addition/Subtraction
- Multiplication/Division
- Exponentiation
- Comparison

## Results
