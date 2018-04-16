/*
** EPITECH PROJECT, 2018
** 32
** File description:
** 32
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

static void get_machine_name32(void)
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

static int set_strtab32(int i)
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
			printf(" %04x ", (unsigned int)objdump.shd32[i].sh_addr);
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
		if (is_ok32((char*)(sh_strtab_p + objdump.shd32[i].sh_name), 0)) {
			printf("Contents of section %s:\n",
				(char*)(sh_strtab_p + objdump.shd32[i].sh_name));
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