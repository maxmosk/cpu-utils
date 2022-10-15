in
in
in
call :solve_quad
hlt


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
abs:            ; a
dup             ; a a
push 0          ; a a 0
jb :absneg      ; a
ret
absneg:
push -1         ; a -1
mul             ; -a
ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
D:              ; rax = a, rbx = b, rcx = c
push rbx        ; b
dup             ; b b
mul             ; b^2
push 4          ; b^2 4
push rax        ; b^2 4 a
push rcx        ; b^2 4 a c
mul             ; b^2 4 a*c
mul             ; b^2 4*a*c
sub             ; b^2-4*a*c
ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
solve_quad:     ; a b c
pop rcx         ; a b
pop rbx         ; a
pop rax         ;
push rax        ; a
push rbx        ; a b
push rcx        ; a b c

push rax        ; a b c
call :abs       ; a b c |a|
push 0.001      ; a b c |a| 0
jb :linear      ; a b c

call :D         ; a b c D
dup             ; a b c D D
call :abs       ; a b c D |D|
push 0.001      ; a b c D |D| 0.001
jb :one_root    ; a b c D

ret

one_root:       ; D
pop rdx         ;
push -1         ; -1
push rbx        ; -1 b
mul             ; -b
push 2          ; -b 2
div             ; -b/2
out             
ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
linear:         ; rbx = b, rcx = c
push rbx        ; b
dup             ; b b
call :abs       ; b |b|
push 0.001      ; b |b| 0.001
jb :zero_b      ; b
pop rbx         ;
push 0          ; 0
push rcx        ; 0 c
sub             ; -c
push rbx        ; -c b
div             ; -c/b
out             ;
ret

zero_b:         ; 0, rbx = 0, rcx = c
pop rbx         ;
push rcx        ; c
call :abs       ; |c|
push 0.001      ; |c| 0.001
jb :zero_c      ;
push NAN        ; NAN
out
ret

zero_c:
push INF
out
ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

