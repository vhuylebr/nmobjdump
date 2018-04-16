/*
** EPITECH PROJECT, 2018
** 32
** File description:
** 32
*/

#include "my_objdump.h"

void print_end32(unsigned int y, int i, char *str)
{
	int padding = 0;

	if (!(y % 16) && y == objdump.shd32[i].sh_size) {
		padding = 0;
	} else {
		padding = (16 - (int)objdump.shd32[i].sh_size % 16) * 2;
		printf("%*s", padding + padding / 9, " ");
	}
	print_dec(y, str, objdump.shd32[i].sh_size);
	printf("\n");
}

void print_hex32(int i)
{
	char *str = (char*)(objdump.buf + objdump.shd32[i].sh_offset);
	unsigned int y = 1;

	while (y <= objdump.shd32[i].sh_size) {
		if (y - 1 == 0)
			printf(" %04x ",
				(unsigned int)objdump.shd32[i].sh_addr);
		printf("%02x", (unsigned char)str[y - 1]);
		if (!(y % 16) && y == objdump.shd32[i].sh_size)
			break;
		(void)y;
		if (!(y % 4) && y > 0 && (y % 16))
			printf(" ");
		else if (!(y % 16) && y > 0) {
			print_dec(y, str, objdump.shd32[i].sh_size);
			printf("\n %04x ",
				(unsigned int)objdump.shd32[i].sh_addr + y);
		}
		++y;
	}
	print_end32(y, i, str);
}

void print_data32(char *sh_strtab_p)
{
	int i = 0;

	for (i = 1; i < objdump.shnum; ++i) {
		if (is_ok32((char*)(sh_strtab_p +
			objdump.shd32[i].sh_name), 0)) {
			printf("Contents of section %s:\n",
				(char*)(sh_strtab_p +
					objdump.shd32[i].sh_name));
			print_hex32(i);
		}
	}
}

void my_objdump32(void)
{
	objdump.elf32 = objdump.buf;
	objdump.shnum = objdump.elf32->e_shnum;
	objdump.shd32 = objdump.buf + objdump.elf32->e_shoff;
	get_machine_name32();
	get_str_tab32();
	printf("\n%s:     file format %s\n", objdump.file_name, "elf32-i386");
	printf("architecture: i386, flags %s\n",
		getflag32());
	printf("start address 0x%08lx\n\n", objdump.elf32->e_entry);
	print_data32(objdump.buf + objdump.str_tab32->sh_offset);
}