in
call :fact
out
hlt

fact:       ; n
dup         ; n n
push 1      ; n n 1
jbe :end    ; n
dup         ; n n
push 1      ; n n 1
sub         ; n n-1
call :fact  ; n n-1 (n-1)!
pop rax     ; n n-1
pop rbx     ; n
dup         ; n n
push rax    ; n n (n-1)!
mul         ; n n!
ret
end:
push 1      ; n 1
ret

