section .data
new_line_char db 10

section .text

string_length:
    xor rax, rax
    .loop:
        cmp byte [rdi + rax], 0
        je .end
        inc rax
        jmp .loop
    .end:
        ret

print_string:
    push rdi           ; save string

    call string_length ; rdi already have string address
    mov rdx, rax       ; string length 

    mov rax, 1         ; write
    mov rsi, rdi       ; where data
    mov rdi, 1         ; stdout
    syscall

    pop rdi
    ret


print_char:
    push rdi      ; first argument
    mov rax, 1    ; write
    mov rsi, rsp  ; where data (in stack now)
    mov rdi, 1    ; stdout
    mov rdx, 1    ; only 1 byte
    syscall
    pop rdi
    ret

print_newline:
    mov rdi, 10
    jmp print_char


print_uint:
    mov rax, rdi      ; div use rax
    mov r9, rsp       ; caller-saved(stack HEAD)

    dec rsp           ; add new row for
    mov byte[rsp], 0  ;  pointer to end of string

    mov r10, 10       ; caller-saved(10 divider)
    .loop:
        xor rdx, rdx  ; will contain ramainder
        div r10       ; div by 10
        add dl, '0'   ; dl - lowest 8bit of rdx

        dec rsp	      ; add new row for
        mov [rsp], dl ;   new digit

        test rax, rax ; check that rax
        jnz .loop     ;   is not 0

    mov rdi, rsp      ; pointer to string head

    push r9           ; save r9
    call print_string ; print as string
    pop r9
    mov rsp, r9       ; return stack HEAD
    ret



print_int:
    test rdi, rdi     ; check that input 
    jns print_uint    ;  uint

    push rdi          ; save rdi
    mov rdi, '-'      ; write
    call print_char   ;  '-'

    pop rdi           ; get rdi
    neg rdi           ;  and convert to negative
    jmp print_uint

string_equals:
    xor rcx, rcx
    .loop:
        mov al, byte[rsi + rcx]
        mov dl, byte[rdi + rcx]
        cmp al, dl 
        jne .not_equal
        cmp al, 0
        je .equal
        inc rcx
        jmp .loop

    .not_equal:
        xor rax, rax
        ret
    .equal:
        mov rax, 1
        ret


read_char:
    push 0       ; stack HEAD -> 0

    mov rdx, 1   ; 1 symvol
    mov rdi, 0   ; stdin
    mov rsi, rsp ; enter to stack HEAD
    mov rax, 0   ; read
    syscall
    pop rax      ; char or zero
    ret 

; rdi points to a string
; returns rax: number, rdx : length
parse_uint:
    xor rcx, rcx ; use rcx because rdx overflow when mul
    xor rax, rax
    mov r10, 10
    xor r9, r9
    .loop:
    	cmp byte[rdi+rcx], '0'     ; check
    	jb .end                    ; number
    	cmp byte[rdi+rcx], '9'     ; between
    	ja .end                    ; 0 and 9
    	mul r10                    ; *10
    	mov r9b,  byte[rdi+rcx]    
    	sub r9b, '0'               ; to number
    	add rax, r9                ; add
    	inc rcx
    	jnz .loop
    .end:
    mov rdx, rcx
    ret

; rdi points to a string
; returns rax: number, rdx : length
parse_int:
    cmp byte[rdi], '-' ; check that non negative
    jne parse_uint

    inc rdi            ; "disable" '-'
    call parse_uint
    neg rax            ; convert
    inc rdx
    ret 


string_copy:
    push rdi
    push rsi
    call string_length
    pop rsi
    pop rdi

    mov rcx, rax             
    inc rcx
    xor rax, rax
    cmp rdx, rcx
    jl .end

    mov rax, rsi
    .loop:
        mov r10, [rdi]
        mov [rsi], r10

        inc rsi
        inc rdi
 
        cmp byte[rdi], 0
        jne .loop

    .end:
    ret

exit:
   mov rax, 60
   syscall


read_word:
    ;rsi    ; buffer size
    ;rdi    ; buffer

    mov r10, -1
    mov r9, rsi;

    .first_whitespaces:
        push rdi
        call read_char
        pop rdi
        
        cmp al, 0
        je .end
        cmp al, 0x21
        jl .first_whitespaces

    inc r10
    cmp r9, r10
    je .bad_end
    mov byte[rdi + r10], al
    cmp rax, 0
    je .end

    .loop:

        push rdi
        call read_char
        pop rdi
        
        cmp rax, 0x21
        jl .end
        inc r10
        cmp r9, r10
        je .bad_end
        mov byte[rdi + r10], al
        cmp al, 0
        je .end
        jmp .loop

    ;.end:    

    ;add rdi, r10
    ;mov byte[rdi], 0
    .end:
        mov byte[rdi+r10+1], 0
        mov rax, rdi
        inc r10
        mov rdx, r10
        ret
    .bad_end:
        xor rax, rax
        inc r10
        mov rdx, r10
        ret


