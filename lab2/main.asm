global _start

extern print_char
extern print_string
extern exit
extern find_word
extern read_word
extern string_length

%include "colon.inc"
section .data
%include "words.inc"

no_word db "Word doesn't exist.", 0

section .text
_start:
    push rbp
    mov rbp, rsp

    mov rcx, 255 ;max
    sub rsp, rcx
    mov rdi, rsp
    mov rsi, rcx
    call read_word

    mov rsi, rax 
    mov rdi, next

    call find_word
    test rax, rax
    jz .error

    .print:
	lea rdx, [rax+8]

	mov rdi, rdx
	push rdx
	call string_length
	pop rdx

	add rdx, rax
	inc rdx ; \0
	mov rdi, rdx
	call print_string
	jmp .end

    .error:
	mov rdi, no_word

	push rdi
	call string_length
	pop rdi

	mov rdx, rax
	mov rsi, rdi
	mov rax, 1
	mov rdi, 2
	syscall

    .end:
	call exit
	mov rsp, rbp
	pop rbp
	ret
