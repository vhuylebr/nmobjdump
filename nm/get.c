/*
** EPITECH PROJECT, 2018
** get
** File description:
** get
*/

#include "my_nm.h"

void get_str_tab(void)
{
	int i;
	
	for (i = 0; i < nm.shnum; ++i) {
		if (nm.shd[i].sh_type == SHT_STRTAB) {
			nm.str_tab = &nm.shd[i];
		}
	}
}

Elf64_Shdr get_section(int macro)
{
	int i;
	
	for (i = 0; i < nm.shnum; ++i) {
		if (nm.shd[i].sh_type == macro) {
			return nm.shd[i];
		}
	}
}
