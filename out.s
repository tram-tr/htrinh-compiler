.file "tests/codegen-sample/good9.bminor"
# DECL
.data
.global x
x:
.quad 5
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
# start of if stmt
# condition expr
# EXPR
	movq $1, %rbx
	cmpq $0, %rbx
	je .L0
# if body
# STMT
# EXPR
# code of id =
# EXPR
	movq $15, %r10
	leaq x, %r11
	movq %r10, (%r11)
	jmp .L1
# false label
.L0:
# else body
# STMT
# EXPR
# code of id =
# EXPR
	movq $10, %r10
	leaq x, %r11
	movq %r10, (%r11)
.L1:
# end of if stmt
# STMT
# start of if stmt
# condition expr
# EXPR
	movq $0, %rbx
	cmpq $0, %rbx
	je .L2
# if body
# STMT
# STMT
# return stmt
# EXPR
	movq x, %r10
	movq %r10, %rax
	jmp .main_epilogue
	jmp .L3
# false label
.L2:
# else body
# STMT
# STMT
# return stmt
# EXPR
	movq $1, %r10
	movq %r10, %rax
	jmp .main_epilogue
.L3:
# end of if stmt
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
