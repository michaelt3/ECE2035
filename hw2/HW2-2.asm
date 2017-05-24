# This routine finds the median of a 100 element array.
# The result is returned in $2.
#
#  June 2016                       <your name here>

.data
Array:  .alloc	100

.text

Median:		addi	$1, $0, Array		# point to array base
		swi	542			# generate random numbers

# your code goes here

		addi $1,$0,0  			# initialize c counter
		addi $2,$0,4 			# initialize j counter
OUTER:	lw $3,Array($1) 		# initialize Nums[c]
		lw $4,Array($2) 		# initialize Nums[j]
		slt $5,$3,$4 			# Nums[c] < nums[j] ? 
		beq $5,$0,INNER 		# If Nums[c] < Nums[j], swap
		sw $4,Array($1)
		sw $3,Array($2)
INNER:  addi $2,$2,4 			# increment j counter
		slti $5,$2,400 			# j < 100 ? 
		bne $5,$0,OUTER 		# continue OUTER loop until j = 100
		addi $1,$1,4 			# else, increment c counter
		addi $2,$0,0 			# reset j counter
		slti $5,$1,400 			# c < 100 ? 
		beq $5,$0,END 			# END if c = 100
		j OUTER 				# Otherwise, loop again
END: 	addi $1,$0,196  		# Nums[49]
		addi $2,$0,200 			# Nums[50]
		lw $1,Array($1) 		# load Num[49]
		lw $2,Array($2) 		# load Num[50]
		add $1,$1,$2 			# Median = Num[49] + Num[50]
		addi $2,$0,2 			
		div $1,$2 				# Median = (Num[49] + Num[50]) / 2
		mflo $1 				# if even, this is the median
		mfhi $2 				# remainder 
		slt $3, $1, $0 			# Median < 0 ? 
		beq $3, $0, END2 		# If negative, run next line
		beq $2, $0, END2		# Odd or even? 
		mflo $2 
		addi $2, $2, 1  		# if negative and odd, add -1
		j REAL
END2:   mflo $2		
REAL:	jr	$31			# otherwise return to caller

