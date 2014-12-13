void haltcpu()
{
	__asm__ __volatile__(
	                     "cpu_halt:\n\t"
	                     "hlt\n\t"
	                     "jmp cpu_halt"
	                     );
}
