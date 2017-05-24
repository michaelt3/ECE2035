#   SortDiamond size-hue sorter
#
#   July 1, 2016                
# 	Michael Troughton

.data
Array:		.alloc 169				# allocate static space for diamond

.text

# comments go here

SortDiamond:		addi	$01, $00, Array		# set memory base
			swi	521			# create sort diamond and update memory
		#	swi	522			# tally errors
		#	swi	523			# redisplay diamond
		#	jr	$31			# return to caller

		# $2 = num
		# $3 = Predicate Register // Initial + spacing // 64 // Array[j-1]%64
		# $4 = Initial
		# $5 = Spacing
		# $6 = a
		# $7 = i
		# $8 = j
		# $9 = j-1
		# $10 = Array[j]
		# $11 = Array[j-1]
		# $12 = Array[j] % 64

# something more is needed ...
			addi $2, $0, 0 		# Initialize num

numLOOP:	slti $3, $2, 4 		# num < 4? // 1 if true, 0 if false
			beq $3, $0, END 	# END if false
			addi $2, $2, 1 		# otherwise, increment num
			addi $4, $0, 24     # initial = 6
			addi $5, $0, 4 		# spacing = 1
			addi $6, $0, 0 		# a = 0
aLOOP:		slti $3, $6, 12 	# a < 12? // 1 if true, 0 if false
			beq $3, $0, DIAM 
checkL6:	slti $3, $6, 6 		# a < 6? // 1 if true, 0 if false
			bne $3, $0, aL6
			addi $3, $0, 6 		
			beq $6, $3, aEQ6 	# if a == 6, branch to aEQ6
aG6:		addi $4, $4, 56		# initial = initial + 14
			addi $5, $5, -8 	# spacing = spacing - 2
			j initI
aL6:		addi $4, $4, 48 	# initial = initial + 12
			addi $5, $5, 8 		# spacing = spacing + 2	
			j initI			
aEQ6: 		addi $4, $0, 368 	# initial = 92
			addi $5, $0, 44 	# spacing = 11	
initI:		add $7, $4, $0 		# i = initial
iLOOP: 		add $3, $4, $5 		# initial + spacing
			slt $3, $7, $3 		# i < initial + spacing? // 1 if true, 0 if false
			beq $3, $0, aINC 	# if false, jump to aINC
			add $8, $7, $0 		# j = i
jLOOP:		slt $3, $4, $8 		# j > initial ? // 1 if true, 0 if false
			beq $3, $0, iINC   
			addi $9, $8, -4 	# j - 1
			lw $10, Array($8)	# load in Array[j]
			lw $11, Array($9)   # load in Array[j-1]
			addi $3, $0, 64 	# constant 64
			div $10, $3
			mfhi $12 			# Array[j] % 64
			div $11, $3
			mfhi $3 			# Array[j-1] % 64
			slt $3, $12, $3 	# Array[j] % 64 < Array[j-1] % 64? // 1 if true, 0 if false
			beq $3, $0 jINC
			sw $11, Array($8)
			sw $10, Array($9)
			j jINC
aINC: 		addi $6, $6, 1		# increment a
			j aLOOP		
iINC: 		addi $7, $7, 4 		# increment i
			j iLOOP
jINC: 		addi $8, $8, -4 	# decrement j
			j jLOOP								 


DIAM: 		addi $4, $0, 312 	# initial = 78
			addi $5, $0, 0 		# spacing = 0
			addi $6, $0, 0 		# a = 0
aLOOP2:		slti $3, $6, 12 	# a < 12? // 1 if true, 0 if false
			beq $3, $0, numLOOP 	# TEMPORARY END, SHOULD BE DIAM
checkL62:	slti $3, $6, 6 		# a < 6? // 1 if true, 0 if false
			bne $3, $0, aL62
			addi $3, $0, 6 		
			beq $6, $3, aEQ62 	# if a == 6, branch to aEQ6
aG62:		addi $4, $4, 56		# initial = initial + 14
			addi $5, $5, -104 	# spacing = spacing - 26
			j initI2
aL62:		addi $4, $4, -48 	# initial = initial - 12
			addi $5, $5, 104 	# spacing = spacing + 26	
			j initI2		
aEQ62: 		addi $4, $0, 80 	# initial = 20
			addi $5, $0, 520 	# spacing = 130			
initI2:		add $7, $4, $0 		# i = initial
iLOOP2: 	add $3, $4, $5 		# initial + spacing
			addi $3, $3, 4 		# initial + spacing + 1
			slt $3, $7, $3 		# i < initial + spacing + 1? // 1 if true, 0 if false
			beq $3, $0, aINC2 	# if false, jump to aINC2
			add $8, $7, $0 		# j = i
jLOOP2:		slt $3, $4, $8 		# j > initial ? // 1 if true, 0 if false
			beq $3, $0, iINC2   
			addi $9, $8, -52 	# j - 13
			lw $10, Array($8)	# load in Array[j]
			lw $11, Array($9)   # load in Array[j-1]
			srl $12, $10, 6
			srl $3, $11, 6
			slt $3, $12, $3 	# Array[j] % 64 < Array[j-1] % 64? // 1 if true, 0 if false
			beq $3, $0 jINC2
			sw $11, Array($8)
			sw $10, Array($9)
			j jINC2
aINC2: 		addi $6, $6, 1		# increment a
			j aLOOP2		
iINC2: 		addi $7, $7, 52 	# increment i
			j iLOOP2
jINC2: 		addi $8, $8, -52 	# decrement j
			j jLOOP2	

END:		jr $31















