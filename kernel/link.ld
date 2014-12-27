ENTRY(_start)
SECTIONS
{
	.text :
	{
		_code = .;
		*(.text)
		*(.rodata*)
		. = ALIGN(4096);
	}

	.data :
	{
		_data = .;
		*(.data)
		. = ALIGN(4096);
	}

	.bss :
	{
		_bss = .;
		*(.bss)
		*(COMMON)
		*(.stack)
		. = ALIGN(4096);
	}

	_end = .;

	/DISCARD/ :
	{
		*(.comment)
	}
}
