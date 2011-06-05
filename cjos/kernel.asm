;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;                                           ;;
;; fasm example.By Craig Bamford (Dex) 2002. ;;
;; 640*480 8bpp (256 colors) vesa1           ;;
;; C:\fasm vesa1.asm vesa1.com               ;;
;;                                           ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
times 100h db 0

Start:
        mov ax,4f02h
        mov bx,101h
        int 10h

        mov dx,0xa000
        mov es,dx
        call window

StartAgain:
        xor dx,dx
MainLoop:
        push dx
        call window
        xor bx,bx
        mov al,0x00
        call static
        call PutTextB
        call PutTextS
        pop dx
        cmp dx,4
        je StartAgain
        inc dx
        mov ah,00h
        int 16h
        jz MainLoop
        mov ax,0003h
        int 10h
        ret

window:
        mov ax,4f05h   
        mov bx,0
        int 10h
        xor bx,bx
        ret

static:
        stosb
        inc bx
        cmp bx,0x00000
        jne static
        ret

PutTopBackGround:
        xor di,di
        mov cx,0x1900
        mov al,0x81
        rep stosb
        ret
                     
PutBottomBackGround:
        mov di,58240
        mov cx,0x17c0
        mov al,0x81
        rep stosb
        ret

DrawImage:
        mov di,bx
        mov dl,[Row]
        cld
GetData:     
        lodsw
        mov bx,ax
        mov cl,16
CheckBit:
        test bx,0x8000
        jz ZeroBit
        mov al,dh   
        stosb
        jmp Skip                   
ZeroBit:
        inc di
Skip:
        shl bx,1
        loop CheckBit         
        add di,624
        dec dl
        jnz  GetData
        ret

PutTextB:
        mov byte [Row],16
        xor di,di
        mov di,716
        mov bx,di
        mov SI,TextS
        mov dh,0x0a
        call DrawImage

        xor di,di
        mov di,733
        mov bx,di
        mov SI,TextP
        mov dh,0x0a
        call DrawImage

        xor di,di
        mov di,750
        mov bx,di
        mov SI,TextA
        mov dh,0x0a
        call DrawImage

        xor di,di
        mov di,767
        mov bx,di
        mov SI,TextC
        mov dh,0x0a
        call DrawImage

        xor di,di
        mov di,784
        mov bx,di
        mov SI,TextE
        mov dh,0x0a
        call DrawImage

        xor di,di
        mov di,813
        mov bx,di
        mov SI,TextP
        mov dh,0x0a
        call DrawImage

        xor di,di
        mov di,830
        mov bx,di
        mov SI,TextO
        mov dh,0x0a
        call DrawImage

        xor di,di
        mov di,847
        mov bx,di
        mov SI,TextN
        mov dh,0x0a
        call DrawImage

        xor di,di
        mov di,864
        mov bx,di
        mov SI,TextG
        mov dh,0x0a
        call DrawImage
        ret
   
PutTextS:
        mov byte [Row],7
        mov di,60234
        mov bx,di
        mov si,LetterB
        mov dh,0ah
        call DrawImage

        mov di,60241
        mov bx,di
        mov si,LetterY
        mov dh,0ah
        call DrawImage
   
        mov di,60255
        mov bx,di
        mov si,LetterC
        mov dh,0ah
        call DrawImage
     
        mov di,60262
        mov bx,di
        mov si,LetterR
        mov dh,0ah
        call DrawImage

        mov di,60269
        mov bx,di
        mov si,LetterA
        mov dh,0ah
        call DrawImage

        mov di,60276
        mov bx,di
        mov si,LetterI
        mov dh,0ah
        call DrawImage

        mov di,60284
        mov bx,di
        mov si,LetterG
        mov dh,0ah
        call DrawImage

        mov di,60298
        mov bx,di
        mov si,LetterB
        mov dh,0ah
        call DrawImage

        mov di,60305
        mov bx,di
        mov si,LetterA
        mov dh,0ah
        call DrawImage

        mov di,60312
        mov bx,di
        mov si,LetterM
        mov dh,0ah
        call DrawImage

        mov di,60319
        mov bx,di
        mov si,LetterF
        mov dh,0ah
        call DrawImage

        mov di,60326
        mov bx,di
        mov si,LetterO
        mov dh,0ah
        call DrawImage

        mov di,60333
        mov bx,di
        mov si,LetterR
        mov dh,0ah
        call DrawImage

        mov di,60340
        mov bx,di
        mov si,LetterD
        mov dh,0ah
        call DrawImage

        mov di,60354
        mov bx,di
        mov si,Two
        mov dh,0ah
        call DrawImage

        mov di,60361
        mov bx,di
        mov si,Zero
        mov dh,0ah
        call DrawImage
   
        mov di,60368
        mov bx,di
        mov si,Zero
        mov dh,0ah
        call DrawImage

        mov di,60375
        mov bx,di
        mov si,Three
        mov dh,0ah
        call DrawImage

        mov di,60385
        mov bx,di
        mov si,CopyrightSymbol
        mov dh,0ah
        call DrawImage
        ret

Row      DB         0
TextA    DW         0FFFFh,08001h,08001h,08001h,087E1h,08421h,08421h,087E1h
         DW         08001h,08001h,087E1h,08421h,08421h,08421h,08421h,0FC3Fh
TextC    DW         0FFFEh,08002h,08002h,087E2h,0843Eh,08400h,08400h,08400h
         DW         08400h,08400h,0843Eh,08422h,087E2h,08002h,08002h,0FFFEh
TextE    DW         0FFFFh,08001h,08001h,087FFh,08400h,08400h,087FEh,08002h
         DW         08002h,087FEh,08400h,08400h,087FFh,08001h,08001h,0FFFFh
TextS    DW         0FFFFh,08001h,08001h,087E1h,0843Fh,08400h,087FFh,08001h
         DW         08001h,0FFE1h,00021h,0FC21h,087E1h,08001h,08001h,0FFFFh
TextM1   DW         0FFFFh,08000h,08000h,08000h,09FE7h,09024h,09024h,09024h
         DW         09024h,09024h,09024h,09024h,09024h,09024h,09024h,0F03Ch
TextM2   DW         0FF00h,00100h,00100h,00100h,0F900h,00900h,00900h,00900h
         DW         00900h,00900h,00900h,00900h,00900h,00900h,00900h,00F00h
TextP    DW         0FFFFh,08001h,08001h,087F9h,08409h,08409h,08409h,087F9h
         DW         08001h,08001h,087FFh,08400h,08400h,08400h,08400h,0FC00h
TextO    DW         0FFFFh,08001h,08001h,087E1h,08421h,08421h,08421h,08421h
         DW         08421h,08421h,08421h,08421h,087E1h,08001h,08001h,0FFFFh 
TextN    DW         0FFFFh,08001h,08001h,087E1h,08421h,08421h,08421h,08421h
         DW         08421h,08421h,08421h,08421h,08421h,08421h,08421h,0FC3Fh
TextG    DW         0FFFEh,08002h,08002h,087E2h,0843Eh,08400h,08400h,08400h
         DW         0847Fh,08441h,08463h,08422h,087E2h,08002h,08002h,0FFFEh
Two      DW         3800h,4400h,0400h,1800h,2000h,4000h,7c00h
Zero     DW         3800h,4400h,4c00h,5400h,6400h,4400h,3800h
Three    DW         3800h,4400h,0400h,1800h,0400h,4400h,3800h
LetterA  DW         3800h,4400h,4400h,7c00h,4400h,4400h,4400h
LetterB  DW         7800h,4400h,4400h,7800h,4400h,4400h,7800h
LetterC  DW         3800h,4400h,4000h,4000h,4000h,4400h,3800h
LetterD  DW         7800h,4400h,4400h,4400h,4400h,4400h,7800h
LetterE  DW         7c00h,4000h,4000h,7800h,4000h,4000h,7c00h
LetterF  DW         7c00h,4000h,4000h,7800h,4000h,4000h,4000h
LetterG  DW         3800h,4400h,4000h,5c00h,4400h,4400h,3800h
LetterH  DW         4400h,4400h,4400h,7c00h,4400h,4400h,4400h
LetterI  DW         7c00h,1000h,1000h,1000h,1000h,1000h,7c00h
LetterJ  DW         0400h,0400h,0400h,0400h,0400h,4400h,3800h
LetterK  DW         4400h,4800h,5000h,6000h,5000h,4800h,4400h
LetterL  DW         4000h,4000h,4000h,4000h,4000h,4000h,7c00h
LetterM  DW         4400h,6c00h,5400h,4400h,4400h,4400h,4400h
LetterN  DW         4400h,6400h,5400h,4c00h,4400h,4400h,4400h
LetterO  DW         3800h,4400h,4400h,4400h,4400h,4400h,3800h
LetterP  DW         7800h,4400h,4400h,7800h,4000h,4000h,4000h
LetterQ  DW         3800h,4400h,4400h,4400h,4400h,4c00h,3c00h
LetterR  DW         7800h,4400h,4400h,7800h,4400h,4400h,4400h
LetterS  DW         3800h,4400h,4000h,3800h,0400h,4400h,3800h
LetterT  DW         7c00h,1000h,1000h,1000h,1000h,1000h,1000h
LetterU  DW         4400h,4400h,4400h,4400h,4400h,4400h,3800h
LetterV  DW         4400h,4400h,4400h,4400h,4400h,2800h,1000h
LetterW  DW         4400h,4400h,4400h,4400h,5400h,6c00h,4400h
LetterX  DW         4400h,4400h,2800h,1000h,2800h,4400h,4400h
LetterY  DW         4400h,4400h,2800h,1000h,1000h,1000h,1000h
LetterZ  DW         7c00h,0400h,0800h,1000h,2000h,4000h,7c00h
CopyrightSymbol DW  1e00h,2100h,4c80h,4880h,4c80h,2100h,1e00h

times 64*1024 - ($-$$) db 0
