.intel_syntax noprefix
.code64

.section .text

.extern setpaging

.globl initpaging
initpaging:
	call setpaging
	cmp rax,0
	je initpaging_err
	mov cr3,rax
	mov al,1
	ret
initpaging_err:
	mov al,0
	ret

.globl invlpaging
invlpaging:
	mov rax,cr3
	mov cr3,rax
	ret
