in
in
in
call :solveQuad
hlt

abs:            ; a
dup             ; a a
dup             ; a a a
push 0          ; a a a 0
jb :neg         ; a a
ret
neg:
push -1 
mul
ret

solveQuad:      ; a b c
pop rcx         ; a b
pop rbx         ; a
pop rax         ;
push rax        ; a
push rbx        ; a b
push rcx        ; a b c

push rax        ; a b c a
call :abs       ; a b c |a|
out

ret

