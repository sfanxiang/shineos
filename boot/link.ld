ENTRY(_start)
SECTIONS
{
	.text :
	{
		_code = .;
		*(.text)
		*(.rodata*)
		. = ALIGN(1024);
	}

	.data :
	{
		_data = .;
		*(.data)
		. = ALIGN(1024);
	}

	.bss :
	{
		_bss = .;
		*(.bss)
		*(COMMON)
		. = ALIGN(1024);
	}

	_end = .;

	/DISCARD/ :
	{
		*(.comment)
	}
}
