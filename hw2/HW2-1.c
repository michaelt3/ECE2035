/*     Median of 100 element integer array

This program finds the median value in a 100 element integer array.

June 2016	     
Michael Troughton 
*/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
   int	Nums[100];
   int	NumNums, Median;
   int  Load_Mem(char *, int *);
    

   if (argc != 3) {
     printf("usage: ./HW2-1 valuefile medianfile\n");
     exit(1);
   }
   NumNums = Load_Mem(argv[1], Nums);
   if (NumNums != 100) {
      printf("valuefiles must contain 100 entries\n");
      exit(1);
   }

   /* Your program goes here */

   int c;          // Counter for loop
   int j;          // Counter for loop 
   int temp;
   for (c = 0; c < 100; c++)
    {
        for (j = c + 1; j < 100; ++j)
        {
            if (Nums[c] > Nums[j])          // Test if values need to be swapped
            {
                temp =  Nums[c];            // Code to swap values 
                Nums[c] = Nums[j];
                Nums[j] = temp;
            }
        }
    }
    Median = (Nums[50]+Nums[49])/2;

   printf("The median of the array is %d\n", Median);

   FILE *fp;
   fp = fopen(argv[2],"w");
   fprintf(fp,"%d",Median);
   fclose(fp);

   exit(0);
}

/* This routine loads in up to 100 newline delimited integers from
a named file in the local directory. The values are placed in the
passed integer array. The number of input integers is returned. */

int Load_Mem(char *InputFileName, int IntArray[]) {
   int	N, Addr, Value, NumVals;
   FILE	*FP;

   FP = fopen(InputFileName, "r");
   if (FP == NULL) {
      printf("%s could not be opened; check the filename\n", InputFileName);
      return 0;
   } else {
      for (N=0; N < 100; N++) {
         NumVals = fscanf(FP, "%d: %d", &Addr, &Value);
         if (NumVals == 2)
            IntArray[N] = Value;
         else
            break;
      }
      fclose(FP);
      return N;
   }
}
