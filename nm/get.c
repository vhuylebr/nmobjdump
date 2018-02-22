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
	char *name_s;

	for (i = 0; i < nm.shnum; ++i) {
		if (nm.shd[i].sh_type == SHT_STRTAB) {
			nm.str_tab = &nm.shd[i];
			name_s = (char*)nm.buf + nm.str_tab->sh_offset
				+ nm.shd[i].sh_name;
			if (!strcmp(name_s, ".shstrtab") && nm.osn == 0) {
				nm.osn = nm.str_tab->sh_offset;
				break;
			}
		}
	}
	for (i = 0; i < nm.shnum; ++i) {
		name_s = (char*)nm.buf + nm.osn + nm.shd[i].sh_name;
		if (!strcmp(name_s, ".strtab")) {
			nm.str_tab = &nm.shd[i];
			return;
		}
	}
}

Elf64_Shdr get_section(int macro)
{
	int i;

	for (i = 0; i < nm.shnum; ++i) {
		if (nm.shd[i].sh_type == (unsigned int)macro) {
			return nm.shd[i];
		}
	}
	return nm.shd[i];
}
