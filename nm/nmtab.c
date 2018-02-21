/*
** EPITECH PROJECT, 2018
** nm
** File description:
** nm
*/

#include "my_nm.h"

static int my_strcmp_nm(char *s, char *s2)
{
	int i = 0;
	int y = 0;

	if (!s || !s2)
		return 0;
	while (s[i] == '_')
		++i;
	while (s2[y] == '_')
		y++;
	while (tolower(s2[y]) == tolower(s[i]) && s2[y] && s[i]) {
		++y;
		++i;
		while (s[i] == '_')
			++i;
		while (s2[y] == '_')
			y++;
	}
	return (tolower(s[i]) - tolower(s2[y]));
}

void sort_nmtab()
{
	int i = 0;
	nmtab_t tmp;

	while (i < nm.nb_func) {
		if (my_strcmp_nm(nm.nmtab[i].name, nm.nmtab[i + 1].name) > 0) {
			tmp = nm.nmtab[i];
			nm.nmtab[i] = nm.nmtab[i + 1];
			nm.nmtab[i + 1] = tmp;
			i = 0;
		} else
			++i;
	}
}

void aff_nmtab()
{
	int i = 0;

	while (i < nm.nb_func && nm.nmtab[i].name) {
		if (nm.nmtab[i].value != -1)
			printf("%016x %c %s\n", nm.nmtab[i].value,
				nm.nmtab[i].type, nm.nmtab[i].name);
		else
			printf("%18c %s\n", nm.nmtab[i].type,
				nm.nmtab[i].name);
		++i;
	}
}

void fill_nmtab()
{
	int y = 0;
	int i = 0;

	for (y = 1; y < nm.nb_func; ++y) {
		if (nm.symtab[y].st_info != STT_SECTION &&
			nm.symtab[y].st_info != STT_FILE) {
			nm.nmtab[i].name = (char *)(nm.buf +
				nm.str_tab->sh_offset + nm.symtab[y].st_name);
			nm.nmtab[i].type = print_type(nm.symtab[y], nm.shd);
			if (nm.symtab[y].st_shndx == SHN_UNDEF)
				nm.nmtab[i].value = -1;
			else
				nm.nmtab[i].value = nm.symtab[y].st_value;
			++i;
		}
	}
}
