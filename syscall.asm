EXTERN syscallNumber:QWORD
EXTERN syscallArgv:QWORD
EXTERN syscallRet:QWORD

.data

.code
asmSyscall PROC
	mov r10, rcx
	mov rcx, [syscallArgv+0]
	mov rdx, [syscallArgv+8]
	mov r8, [syscallArgv+16]
	mov r9, [syscallArgv+24]
	mov rax, [syscallArgv+32]
	push rax
	mov rax, [syscallArgv+40]
	push rax
	mov rax, [syscallArgv+48]
	push rax
	mov rax, [syscallArgv+56]
	push rax
	mov rax, [syscallArgv+64]
	push rax
	mov rax, [syscallArgv+72]
	push rax
	mov rax, [syscallArgv+80]
	push rax
	mov rax, [syscallArgv+88]
	push rax
	mov rax, [syscallArgv+96]
	push rax
	mov rax, [syscallArgv+104]
	push rax
	mov rax, [syscallArgv+112]
	push rax
	mov rax, [syscallArgv+120]
	push rax
	mov rax, [syscallArgv+128]
	push rax
	mov rax, [syscallNumber]
	syscall
	mov [syscallRet], rax
	add rsp, 104
	ret
asmSyscall ENDP

END