/*
** EPITECH PROJECT, 2018
** printtype
** File description:
** printype
*/

#include "my_nm.h"

void prog_types(Elf64_Sym sym, char *c, Elf64_Shdr *shdr)
{
	if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS) {
		switch (shdr[sym.st_shndx].sh_flags) {
			case SHF_ALLOC:
				*c = 'R';
				break;
			case SHF_ALLOC | SHF_WRITE:
				*c = 'D';
				break;
			case SHF_ALLOC | SHF_EXECINSTR:
				*c = 'T';
				break;
		}
	}

}

void type_types(Elf64_Sym sym, char *c, Elf64_Shdr *shdr)
{
	switch (shdr[sym.st_shndx].sh_type) {
		case SHT_INIT_ARRAY:
			*c = 't';
			break;
		case SHT_DYNAMIC:
			*c = 'D';
			break;
		case SHT_FINI_ARRAY:
			*c = 't';
			break;
		case SHT_NOBITS:
			if (shdr[sym.st_shndx].sh_flags ==
				(SHF_ALLOC | SHF_WRITE))
				*c = 'B';
			break;
		case SHT_GROUP:
			*c = 'N';
	}
	if (*c == 0)
		prog_types(sym, c, shdr);
}

void shndx_type(Elf64_Sym sym, char *c, Elf64_Shdr *shdr)
{
	switch (sym.st_shndx) {
		case SHN_UNDEF:
			*c = 'U';
			break;
		case SHN_ABS:
			*c = 'A';
			break;
		case SHN_COMMON:
			*c = 'C';
			break;
	}
	if (*c == 0)
		type_types(sym, c, shdr);
}

void info_type(Elf64_Sym sym, char *c, Elf64_Shdr *shdr)
{
	switch (ELF64_ST_BIND(sym.st_info)) {
		case STB_GNU_UNIQUE:
			*c = 'u';
			break;
		case STB_WEAK:
			if (ELF64_ST_BIND(sym.st_info) == STB_WEAK)
				*c = 'W';
			if (ELF64_ST_TYPE(sym.st_info) == STT_OBJECT) {
				*c = 'V';
			}
			if (sym.st_shndx == SHN_UNDEF)
				*c = *c + 32;
			break;
	}
	if (*c == 0)
		shndx_type(sym, c, shdr);
}

char print_type(Elf64_Sym sym, Elf64_Shdr *shdr)
{
	char  c = 0;

	info_type(sym, &c, shdr);
	if (c == 0)
		c = '?';
	else if (ELF64_ST_BIND(sym.st_info) == STB_LOCAL && c != 't')
		c += 32;
	return (c);
}
