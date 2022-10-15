push 80
pop rax
push 24
pop rbx

push 0          ; 0
pop rcx         ;
y_start:

push 0          ; 0
pop rdx         ;
x_start:
push rdx        ; x

push rax        ; x X_SIZE
push rcx        ; x X_SIZE y
mul             ; x X_SIZE*y
push rdx        ; x X_SIZE*y x
add             ; x X_SIZE*y+x
pop rdx         ; x
push 49         ; x 49
pop [rdx]       ; x
dup             ; x x
pop rdx         ; x


call :dist      ; x d
push 5          ; x d 5
ja :next_x      ; x

push rax        ; x X_SIZE
push rcx        ; x X_SIZE y
mul             ; x X_SIZE*y
push rdx        ; x X_SIZE*y x
add             ; x X_SIZE*y+x
pop rdx         ; x
push 48         ; x 48
pop [rdx]       ; x

next_x:
push 1          ; x 1
add             ; x+1
dup             ; x+1 x+1
pop rdx         ; x+1
push rax        ; x+1 X_SIZE
jbe :x_start

push rcx        ; y
push 1          ; y 1
add             ; y+1
dup             ; y+1 y+1
pop rcx         ; y+1
push rbx        ; y+1 Y_SIZE
print
jbe :y_start

print
hlt

dist:           ; rcx=y, rdx=x
push rcx        ; y
push 12         ; y y0
sub             ; y-y0
dup             ; y-y0 y-y0
mul             ; Dy
push rdx        ; Dy x
push 25         ; Dy x x0
sub             ; Dy x-x0
dup             ; Dy x-x0 x-x0
mul             ; Dy Dx

add             ; Dy+Dx
sqrt            ; d
ret


