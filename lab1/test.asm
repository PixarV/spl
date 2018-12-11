global _start
section .data
    test: dq -1

section .text
_start:
    mov r3w, -1
    mov qword[test], 42
    mov rdi, 60
    syscall
