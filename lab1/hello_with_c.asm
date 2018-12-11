global main
extern printf 


section .data

hello db "Hello, ritt!", 0
fmt db "%s", 0

section .text

main:
    mov rdi, fmt
    mov rsi, hello
    mov rax, 0
    call printf 
    
    ret
    


