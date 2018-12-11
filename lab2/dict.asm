global find_word

extern string_equals

;rsi - str
;rdi - last word
;rax - return addr | 0 
find_word:
    xor rax, rax
    mov r9, rdi

    .loop:
	lea rdi, [r9+8]

	push r9
	push rdi
	push rsi
	call string_equals
	pop rsi
	pop rdi
	pop r9

	cmp rax, 1
	jz .end

	mov r9, [r9]
	test r9, r9
	jnz .loop

    .end:
	mov rax, r9 ; 0 if last
	ret
