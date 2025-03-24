[bits 16]
[org 0x7c00]

start:
    mov ah, 0x0e
    mov al, 'N'
    int 0x10
    mov al, 'O'
    int 0x10
    mov al, 'V'
    int 0x10
    mov al, 'A'
    int 0x10
    mov al, 'O'
    int 0x10
    mov al, 'S'
    int 0x10

hang:
    jmp hang

times 510 - ($ - $$) db 0
dw 0xAA55
