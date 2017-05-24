# HW1-3
# Student Name: Michael Troughton
# Date: June 1, 2016
#
# This program determines the index of SetA[i] if SetA were sorted in ascending order.  where SetA is always of size 10,
# and Then stores the resulting index in the location labelled Result.

# NOTE:
# The program should work on multiple test cases, not just the one provided.
# For full credit, be sure to document your code.

.data
i:  .word 1
SetA:  .word 9, 17, -7, 3, -27, 25, 42, 26, 8, -60
Result: .alloc 1

.text		
		# write your code here...
		lw $1, i($0)
		addi $2, $0, 4 			# Byte Size 4 in $2
		mult $1, $2
		mflo $3 
		lw $3, SetA($3)			# Initial Value in $3
    	addi $4, $0, 0			# Loop Counter in $4
		addi $5, $0, 9			# Constant 9 in $5
		addi $6, $0, 0			# Position Counter in $6
LOOP:   slt $1, $5, $4			# Loop 10 times
		bne $1, $0, EXIT	
		addi $4, $4, 1          # Increment loop counter ($4) by 1	
		mult $4, $2 			# Multiply loop counter ($4) by 4
		mflo $7
		lw $7, SetA($7)			# Load test number in $7
		slt $1, $3, $7			# 1 if initial value < test value
		bne $1, $0, LOOP 		# If initial value < test value, go back to LOOP
		addi $6, $6, 1 			# Else, increment position counter ($6)
		j LOOP
EXIT:   addi $6, $6, -1 		# Corrects for overcounting by 1
		sw $6, Result($0)
		jr $31	     			# return to OS


