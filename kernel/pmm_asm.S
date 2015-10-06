.intel_syntax noprefix
.code64

.globl get_phy_kernel_start
.extern kernel_start
get_phy_kernel_start:
	mov rax,[kernel_start[rip]]
	ret

.globl get_phy_kernel_end
.extern _end
get_phy_kernel_end:
	mov rax,[kernel_start[rip]]
	lea rcx,_end[rip]
	add rax,rcx
	lea rcx,kernel_start[rip]
	sub rax,rcx
	ret
