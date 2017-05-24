/*            Diamond Sort


<date>     	               <your name here>

This program sorts a diamond array of disks of varying hues and
diameters. */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
   int	Array[169];
   int	NumVals;
   int  Load_Mem(char *, int *);
   void	Print_Array(int *);

   if (argc != 2) {
     printf("usage: %s valuefile\n", argv[0]);
     exit(1);
   }
   NumVals = Load_Mem(*++argv, Array);
   if (NumVals != 85) {
      printf("not enough values in %s\n", argv[0]);
      exit(1);
   }

/* your code goes here */   

   Print_Array(Array);
   exit(0);
}

/* This routine initializes a 13 x 13 array with zeros. Then it loads
85 memory elements from the input file. The values are loaded in
address: value tuples. The first value is loaded into position 6. All
other values are positioned relative to the address of the first word.
The number of loaded values are returned. */

int Load_Mem(char *InputFileName, int Array[]) {
   int	N, Addr, Value, Index, NumVals;
   int	Base = -1;
   FILE	*FP;

   for (N=0; N < 169; N++)
     Array[N] = -1;
   FP = fopen(InputFileName, "r");
   if (FP == NULL) {
      printf("%s could not be opened; check the filename\n", InputFileName);
      return 0;
   } else {
      for (N=0; N < 85; N++) {
         NumVals = fscanf(FP, "%d: %d", &Addr, &Value);
         if (NumVals != 2)
            break;
         else
            if (Base == -1)
               Base = Addr - 24;
            Index = (Addr - Base) / 4;
            Array[Index] = Value;
      }
      fclose(FP);
      return N;
   }
}

/* This routine prints a diamond. The hue and diameter are unpacked before printing.
Empty array cells are printed as blanks. */

void Print_Array(int Array[]) {
   int	N, Value, Diameter, Hue;

   for (N=0; N < 169; N++) {
      Value = Array[N];
      if (Value == -1)
         printf("      ");
      else {
         Diameter = Value / 64;
         Hue = Value % 64;
         printf("%02d:%02d ", Diameter, Hue);
      }
      if ((N % 13) == 12)
         printf("\n");
   }
}
