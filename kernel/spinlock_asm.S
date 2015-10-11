.intel_syntax noprefix
.code64

spinlock_wait:
	test dword ptr [rdi],1
	jnz spinlock_wait

.globl spinlock_acquire
spinlock_acquire:
	lock bts dword ptr [rdi],0
	jc spinlock_wait
	ret

.globl spinlock_acquire_nowait
spinlock_acquire_nowait:
	lock bts dword ptr [rdi],0
	jc spinlock_fail
	mov eax,1
	ret
spinlock_fail:
	mov eax,0
	ret

.globl spinlock_release
spinlock_release:
	mov dword ptr [rdi],0
	ret
