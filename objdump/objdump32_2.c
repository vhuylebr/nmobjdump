/*
** EPITECH PROJECT, 2018
** tes
** File description:
** tes
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

void get_machine_name32(void)
{
	int i = 0;

	while (i < 18) {
		if (pc[i].macro == objdump.elf32->e_machine) {
			objdump.machine_name = pc[i].str;
			return ;
		}
		++i;
	}
	objdump.machine_name = "i386:x86-64";
}

int set_strtab32(int i)
{
	char *name_s;

	if (objdump.shd32[i].sh_type == SHT_STRTAB) {
		objdump.str_tab32 = &objdump.shd32[i];
		name_s = (char*)objdump.buf + objdump.str_tab32->sh_offset
			+ objdump.shd32[i].sh_name;
		if (!strcmp(name_s, ".shstrtab")) {
			objdump.str_tab32 = &objdump.shd32[i];
			return (1);
		}
	}
	return (0);
}

void get_str_tab32(void)
{
	int i;

	for (i = 0; i < objdump.shnum; ++i) {
		if (set_strtab32(i))
			return ;
	}
}

char *getflag32(void)
{
	switch (objdump.elf32->e_type) {
		case ET_REL:
			return ("0x00000011:\nHAS_RELOC, HAS_SYMS");
		case ET_EXEC:
			return ("0x00000112:\nEXEC_P, HAS_SYMS, D_PAGED");
		case ET_DYN:
			return ("0x00000150:\nHAS_SYMS, DYNAMIC, D_PAGED");
		case ET_CORE:
			return ("Core\n");
	}
	return ("(null)");
}

int is_ok32(char *str, int i)
{
	if (objdump.elf32->e_type == ET_REL) {
		return (find(9, str, bad_section_o));
	} else if (objdump.elf32->e_type == ET_EXEC) {
		return (find(7, str, bad_section_exec));
	} else {
		return (find(7, str, bad_section_exec));
	}
	return (1);
}
