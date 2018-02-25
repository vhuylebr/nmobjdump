/*
** EPITECH PROJECT, 2018
** printf
** File description:
** print
*/

#include "my_objdump.h"

void print_dec(unsigned int y, char *str, unsigned int max)
{
	unsigned int tmp = y;

	if (y < max)
		y = y - 16;
	else if (!(y % 16) && y == max) {
		y = y - 16;
	} else {
		y = y - max % 16 - 1;
		--tmp;
	}
	printf("  ");
	while (y < tmp) {
		if (isprint(str[y]))
			printf("%c", str[y]);
		else
			printf(".");
		++y;
	}
}

void print_end(unsigned int y, int i, char *str)
{
	int padding = 0;

	if (!(y % 16) && y == objdump.shd[i].sh_size) {
		padding = 0;
	} else {
		padding = (16 - (int)objdump.shd[i].sh_size % 16) * 2;
		printf("%*s", padding + padding / 9, " ");
	}
	print_dec(y, str, objdump.shd[i].sh_size);
	printf("\n");
}

void print_hex(int i)
{
	char *str = (char*)(objdump.buf + objdump.shd[i].sh_offset);
	unsigned int y = 1;

	while (y <= objdump.shd[i].sh_size) {
		if (y - 1 == 0)
			printf(" %04x ", (unsigned int)objdump.shd[i].sh_addr);
		printf("%02x", (unsigned char)str[y - 1]);
		if (!(y % 16) && y == objdump.shd[i].sh_size)
			break;
		(void)y;
		if (!(y % 4) && y > 0 && (y % 16))
			printf(" ");
		else if (!(y % 16) && y > 0) {
			print_dec(y, str, objdump.shd[i].sh_size);
			printf("\n %04x ",
				(unsigned int)objdump.shd[i].sh_addr + y);
		}
		++y;
	}
	print_end(y, i, str);
}

void print_data(char *sh_strtab_p)
{
	int i = 0;

	for (i = 1; i < objdump.shnum; ++i) {
		if (is_ok((char*)(sh_strtab_p + objdump.shd[i].sh_name), 0)) {
			printf("Contents of section %s:\n",
				(char*)(sh_strtab_p + objdump.shd[i].sh_name));
			print_hex(i);
		}
	}
}
