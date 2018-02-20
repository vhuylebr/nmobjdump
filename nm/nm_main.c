/*
** EPITECH PROJECT, 2018
** nm
** File description:
** nm
*/

#include "my_nm.h"

int verif_flag(Elf64_Ehdr *elf)
{
	//printf ("E=%c L=%c F=%c\n", elf->e_ident[1], elf->e_ident[2], elf->e_ident[3]);
	if (elf->e_ident[1] == 'E' && elf->e_ident[2] == 'L'
		  && elf->e_ident[3] == 'F')
		return 1;
	return 0;

}

char            print_type(Elf64_Sym sym, Elf64_Shdr *shdr)
{
	char  c;

	if (ELF64_ST_BIND(sym.st_info) == STB_GNU_UNIQUE)
		c = 'u';
	else if (ELF64_ST_BIND(sym.st_info) == STB_WEAK) {
		c = 'W';
		if (sym.st_shndx == SHN_UNDEF)
			c = 'w';
	}
	else if (ELF64_ST_BIND(sym.st_info) == STB_WEAK && ELF64_ST_TYPE(sym.st_info) == STT_OBJECT) {
		c = 'V';
		if (sym.st_shndx == SHN_UNDEF)
			c = 'v';
	}
	else if (sym.st_shndx == SHN_UNDEF)
		c = 'U';
	else if (sym.st_shndx == SHN_ABS)
		c = 'A';
	else if (sym.st_shndx == SHN_COMMON)
		c = 'C';
	else if (shdr[sym.st_shndx].sh_type == SHT_NOBITS
		&& shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
		c = 'B';
	else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS
			&& shdr[sym.st_shndx].sh_flags == SHF_ALLOC)
		c = 'R';
	else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS
			&& shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
		c = 'D';
	else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS
			&& shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_EXECINSTR))
		c = 'T';
	else if (shdr[sym.st_shndx].sh_type == SHT_INIT_ARRAY || shdr[sym.st_shndx].sh_type == SHT_FINI_ARRAY)
		return 't';
	else if (shdr[sym.st_shndx].sh_type == SHT_DYNAMIC)
		c = 'D';
	else
		c = '?';
	if (ELF64_ST_BIND(sym.st_info) == STB_LOCAL && c != '?')
		c += 32;
	return c;
}

void get_str_tab()
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

int my_strcmp_nm(char *s, char *s2)
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

void my_nm()
{
	int i = 0;
	int y = 0;

	nm.elf = nm.buf;
	nm.shnum = nm.elf->e_shnum;
	nm.shd = nm.buf + nm.elf->e_shoff;
	get_str_tab();
	nm.sect_sym = get_section(SHT_SYMTAB);
	nm.symtab = (Elf64_Sym *)(nm.buf + nm.sect_sym.sh_offset);
	printf("\n%s:\n", nm.file_name);
	nm.nb_func = nm.sect_sym.sh_size / nm.sect_sym.sh_entsize;
	nm.nmtab = malloc(sizeof(nmtab_t) * nm.nb_func + 1);
	for (y = 1; y < nm.nb_func; ++y) {
		if (nm.symtab[y].st_info != STT_SECTION &&
			nm.symtab[y].st_info != STT_FILE) {
			nm.nmtab[i].name = (char *)(nm.buf + nm.str_tab->sh_offset + nm.symtab[y].st_name);
			nm.nmtab[i].type = print_type(nm.symtab[y], nm.shd);
			if (nm.symtab[y].st_shndx == SHN_UNDEF) {
				nm.nmtab[i].value = -1;
				//printf("%18c %s\n", print_type(nm.symtab[y], nm.shd), (char *)(nm.buf + nm.str_tab->sh_offset + nm.symtab[y].st_name));
			}
			else {
				nm.nmtab[i].value = nm.symtab[y].st_value;
				//printf("%016x %c %s\n", (unsigned int)nm.symtab[y].st_value,  print_type(nm.symtab[y], nm.shd), (char *)(nm.buf + nm.str_tab->sh_offset + nm.symtab[y].st_name));
			}
			++i;
		}
	}
	sort_nmtab();
	aff_nmtab();
	free(nm.nmtab);
}

int start()
{
	if (nm.fd != -1) {
		fstat(nm.fd, &nm.s);
		nm.buf = mmap(NULL, nm.s.st_size, PROT_READ, MAP_PRIVATE, nm.fd,
			0);
		if (nm.buf != NULL && verif_flag((Elf64_Ehdr *)nm.buf)) {
			my_nm();
		} else {
			dprintf(2, "my_nm: « %s »: not a valid file.\n",
				nm.file_name);
			return 84;
		}
		munmap(nm.buf, nm.s.st_size);
		close(nm.fd);
	} else {
		dprintf(2, "my_nm: « %s »: file not found.\n", nm.file_name);
		return 84;
	}
}

int main (int ac, char **av)
{
	int i = 1;

	if (ac == 1) {
		nm.fd = open("a.out", O_RDONLY);
		start();
	}
	else {
		while (i < ac) {
			nm.file_name = av[i];
			nm.fd = open(av[i], O_RDONLY);
			start();
			++i;
		}
	}
}
