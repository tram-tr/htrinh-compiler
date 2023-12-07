.file "input.bminor"
# DECL
.data
.global x
x:
.quad 1
# DECL
.text
.global main
main:
# start of function prologue
	pushq %rbp
	movq %rsp, %rbp

	subq $0, %rsp
	pushq %rbx
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
# end of functiop prologue
# STMT
# STMT
# start of for loop
# init expr
.L0:
# condition expr
# loop body
# STMT
# STMT
# start of if stmt
# condition expr
# EXPR
# code of cmp 
# EXPR
	movq x, %rbx
# EXPR
	movq $10, %r10
# CMP
	cmpq %r10, %rbx
	jl .L4
.L5:
	movq $0, %r10
	jmp .L6
.L4:
	movq $1, %r10
	jmp .L6
.L6:
	cmpq $0, %r10
	je .L2
# if body
# STMT
# STMT
# printing exprs
	pushq %r10
	pushq %r11
# EXPR
	movq x, %rbx
	movq %rbx, %rdi
	call print_integer
	popq %r11
	popq %r10
	pushq %r10
	pushq %r11
# EXPR
.data
.L7:
	.string "\n"
.text
	leaq .L7, %rbx
	movq %rbx, %rdi
	call print_string
	popq %r11
	popq %r10
# STMT
# EXPR
# code of ++
# EXPR
	movq x, %rbx
	movq %rbx, %r11
	incq %rbx
	movq %rbx, x
	jmp .L3
# false label
.L2:
# else body
# STMT
# STMT
# return stmt
# EXPR
	movq $10, %rbx
	movq %rbx, %rax
	jmp .main_epilogue
.L3:
# end of if stmt
# next expr
	jmp .L0
.L1:
# end of for loop
# start of function epilogue
.main_epilogue:
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rbx
	movq %rbp, %rsp
	popq %rbp
	ret
# end of function epilogue
