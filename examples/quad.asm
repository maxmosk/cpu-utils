in
in
in
call :solveQuad
hlt

abs:            ; a
dup             ; a a
push 0          ; a a 0
jb :absneg      ; a
ret
absneg:
push -1         ; a -1
mul             ; -a
ret


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


solveQuad:      ; a b c
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

call :D

ret


linear:         ; rbx = b, rcx = c
push rbx        ; b
dup             ; b b
call :abs       ; b |b|
push 0          ; b |b| 0
jb :zero_b      ; b
pop rbx         ;
push 0          ; 0
push rcx        ; 0 c
sub             ; -c
push rbx        ; -c b
div             ; -c/b
out             ;
ret

zero_b:         ; 0
ret

