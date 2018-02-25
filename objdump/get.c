/*
** EPITECH PROJECT, 2018
** obj
** File description:
** obj
*/

#include "my_objdump.h"

static pc_t const pc[20] = {
	{EM_NONE, "None"},
	{EM_M32, "WE32100"},
	{EM_SPARC, "Sparc"},
	{EM_386, "Intel 80386"},
	{EM_68K, "MC68000"},
	{EM_PPC64, "PowerPC64"},
	{EM_88K, "MC88000"},
	{EM_PARISC, "HPPA"},
	{EM_SPARC32PLUS, "Sparc v8+"},
	{EM_PPC, "PowerPC"},
	{EM_S390, "IBM S/390"},
	{EM_MIPS, "MIPS R3000"},
	{EM_ARM, "ARM"},
	{EM_860, "Intel 80860"},
	{EM_SH, "Renesas / SuperH SH"},
	{EM_SPARCV9, "Sparc v9"},
	{EM_IA_64, "Intel IA-64"},
	{EM_X86_64, "i386:x86-64"}
};

static int set_strtab(int i)
{
	char *name_s;

	if (objdump.shd[i].sh_type == SHT_STRTAB) {
		objdump.str_tab = &objdump.shd[i];
		name_s = (char*)objdump.buf + objdump.str_tab->sh_offset
			+ objdump.shd[i].sh_name;
		if (!strcmp(name_s, ".shstrtab")) {
			objdump.str_tab = &objdump.shd[i];
			return (1);
		}
	}
	return (0);
}

void get_machine_name(void)
{
	int i = 0;

	while (i < 18) {
		if (pc[i].macro == objdump.elf->e_machine) {
			objdump.machine_name = pc[i].str;
			return ;
		}
		++i;
	}
	objdump.machine_name = "i386:x86-64";
}

void get_str_tab(void)
{
	int i;

	for (i = 0; i < objdump.shnum; ++i) {
		if (set_strtab(i))
			return ;
	}
}

char *getflag(void)
{
	switch (objdump.elf->e_type) {
		case ET_REL:
			return ("00000011:\nHAS_RELOC, HAS_SYMS");
		case ET_EXEC:
			return ("00000112:\nEXEC_P, HAS_SYMS, D_PAGED");
		case ET_DYN:
			return ("00000150:\nHAS_SYMS, DYNAMIC, D_PAGED");
		case ET_CORE:
			return ("Core\n");
	}
	return ("(null)");
}

int find(int size, char *str, const char **str2)
{
	int i = 0;

	while (i < size) {
		if (!strcmp(str, str2[i]))
			return (0);
		++i;
	}
	return (1);
}
