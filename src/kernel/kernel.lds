
OUTPUT_ARCH("i386")
ENTRY(_start)
SECTIONS 
{
	. = 0x100000;
	.text :
	{
		_text = .;
		*(.text)
		_etext = .;
	}
	. = ALIGN(8);
	.data :
	{
		_data = .;
		*(.data)
		_edata = .;
	}
	.bss :
	{
		_bss = .;
		*(.bss)
		_ebss = .;
	}

	_end = .;
}
