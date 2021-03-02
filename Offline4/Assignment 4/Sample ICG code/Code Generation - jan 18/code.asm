mov ax, i
cmp ax, 2
jl L0
mov t0, 0
jmp L1
L0:
mov t0, 1
L1:
mov ax, t0
cmp ax, 0
je L2
L2:
