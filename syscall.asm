EXTERN syscallNumber:QWORD
EXTERN syscallArgv:QWORD
EXTERN syscallRet:QWORD
EXTERN fakeStack:QWORD

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
	mov rax, [syscallNumber]
	syscall
	mov [syscallRet], rax
	add rsp, 32
	ret
asmSyscall ENDP

END