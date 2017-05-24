#include <stdio.h>
#include <stdlib.h>

/* 
 Student Name: Michael Troughton	
 Date: June 1, 2016

ECE 2035 Homework 1-2

This is the only file that should be modified for the C implementation
of Homework 1.

This program determines the  index of SetA[i] if SetA were sorted in ascending order.  where SetA is always of size 10,
and prints SetA[i] is ??, and if setA were sorted it would be in SetA[?] 
*/

// FOR FULL CREDIT, BE SURE TO TRY MULTIPLE TEST CASES and DOCUMENT YOUR CODE.
// WE WILL RUN DIFFERENT TEST CASES
int i = 6;
int temp = 0;
int SetA[] = {9, 17, -7, 3, -27, 25, 42, 26, 8, -60};
int counter = 0;
int c;          // Counter for loop
int j;          // Counter for loop

int main() 
{
    int initial = SetA[i];                  // Initial value of SetA[i]
	for (c = 0; c < 10; c++)
    {
        for (j = c + 1; j < 10; ++j)
        {
            if (SetA[c] > SetA[j])          // Test if values need to be swapped
            {
                temp =  SetA[c];            // Code to swap values 
                SetA[c] = SetA[j];
                SetA[j] = temp;
            }
        }
    }
    
    for (int a = 0; a < 10; a++)            // Counter needed to find end position
    {
    	if (SetA[a] == initial)
    		break;
    	else
    		counter++;
    }
  printf("SetA[%d] is %d, and if setA were sorted it would be in SetA[%d]\n",i , initial, counter);

  return 0;
}


