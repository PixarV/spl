global _start

section .text

; mov rdi, str
; ret: rax 
string_length:
    xor rax, rax
    .loop:
	cmp byte[rdi+rax], 0
	jz .end
	inc rax
	jmp .loop

    .end:
	ret

; mov rdi, str
print_string:
    xor rax, rax
    push rdi
    call string_length
    pop rdi
    mov rdx, rax
    mov rsi, rdi
    call print
    ret


; mov rdi, char
print_char:
    xor rax, rax
    push di
    mov rsi, rsp
    mov rdx, 1
    call print
    pop di
    ret

print_newline:
    xor rax, rax
    mov rdi, 0x13
    call print_char
    ret

; mov rdi, num (u)
print_uint:
    mov rax, rdi
    mov r8, rsp 
    mov r10, 10
    
    push 0
    .loop:
	xor rdx, rdx
	div r10 
	or rdx, 0x30
	dec rsp
	mov byte[rsp], dl

	test rax, rax
	jnz .loop
    
    mov rdi, rsp
    push r8
    call print_string
    pop r8

    mov rsp, r8 
    ret


; mov rdi, num (s)
print_int:
    cmp rdi, 0
    jge .end

    neg rdi
    push rdi
    mov rdi, '-'
    call print_char
    pop rdi
    jmp .end

    .end:
	call print_uint
        ret

; rdi - str1
; rsi - str2
; rax - 0 | 1
string_equals:
    mov rax, 1
    xor r8, r8
    .loop:
	mov cl, [rdi+r8]
	cmp cl, [rsi+r8]
	jnz .fail
	inc r8
	test cl, cl 
	jz .end
	jmp .loop
    
    .fail:
	xor rax, rax
    .end:
	ret

; ret - rax
read_char:
    xor rax, rax
    push 0
    mov rsi, rsp
    mov rdx, 1
    call read
    mov rax, [rsp]
    pop rdx
    ret 

; mov rdi, buf
; mov rsi, count
; ret buf -> rax
;
read_word:
    mov r9, rdi
    mov r10, rsi

    xor rcx, rcx
    dec r10 ; \0
    .loop:
	push r9
	push rcx
	call read_char
	pop rcx
	pop r9

	cmp al, 10
	jz .loop
	cmp al, 9
	jz .loop
	cmp al, 0x20
	jz .loop

	.word_loop:
	    mov [r9+rcx], al
	    inc rcx

	    push rcx
	    call read_char
	    pop rcx

	    cmp al, 10
	    jz .end
	    cmp al, 9
	    jz .end
	    cmp al, 0x20
	    jz .end
	    cmp al, 0
	    jz .end

	    cmp rcx, r10
	    jnz .word_loop

    xor rax, rax
    ret

    .end:
	mov byte[r9+rcx], 0
	mov rax, r9
	mov rdx, rcx
	ret


; rdi points to a string
; returns rax: number, rdx : length
parse_uint:
    xor rax, rax
    xor rcx, rcx
    xor r9, r9
    mov r10, 10

    .loop:
	mov r9b, [rdi+rcx]
	cmp r9b, 0x30
	jb .end 
	cmp r9b, 0x39
	ja .end 

	mul r10
	and r9, 0xF
	add rax, r9
	inc rcx

	jmp .loop
	
    .end:
	mov rdx, rcx
	ret

; rdi points to a string
; returns rax: number, rdx : length
parse_int:
    xor rax, rax
    xor rdx, rdx

    cmp byte[rdi], '-'
    jz .neg_num
    
    call parse_uint
    jmp .end

    .neg_num:
	inc rdi
	call parse_uint
	neg rax
	inc rdx

    .end:
	ret 

; mov rdi, src  
; mov rsi, buf
; rdx - len
; ret rsi or 0
string_copy:
    call string_length
    cmp rdx, rax ; with len
    jg .loop
    xor rax, rax 
    jmp .end
    
    .loop:
    mov r8, rsi
    mov rcx, rdx
    xor rdx, rdx
    mov rdx, rdi
    mov rdi, rsi
    mov rsi, rdx
    
    repnz movsb

    mov rax, r8
    .end:
	ret

; Util

exit:
    mov rax, 60
    xor rdi, rdi 
    syscall

print:
    mov rax, 1
    mov rdi, 1
    syscall
    ret

read:
    mov rax, 0
    mov rdi, 0
    syscall
    ret

section .data
msg: db "-1dasda", 0 
char: db 'A'
temp: times 20 db 0 

text1: db "qwer", 0
text2: db "qwer", 0

section .text


;_start:
    mov rdi, msg
    mov rsi, temp
    mov rdx, 7
    call string_copy

    call exit
