/*
** EPITECH PROJECT, 2018
** obj
** File description:
** obj
*/

#include "my_objdump.h"

int verif_flag(Elf64_Ehdr *elf)
{
	if (elf->e_ident[1] == 'E' && elf->e_ident[2] == 'L'
		  && elf->e_ident[3] == 'F')
		return 1;
	return 0;

}

static void get_machine_name(void)
{
  	int i = 0;

	pc_t pc[20] = {{EM_NONE, "None"},
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
		{EM_X86_64, "i386:x86-64"}};
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
		if (objdump.shd[i].sh_type == SHT_STRTAB) {
			objdump.str_tab = &objdump.shd[i];
			return;
		}
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
                        printf("Core");
                        break;
        }
}

void my_objdump(void)
{
	int i;

	objdump.elf = objdump.buf;
	objdump.shnum = objdump.elf->e_shnum;
	objdump.shd = objdump.buf + objdump.elf->e_shoff;
	get_machine_name();
	get_str_tab();
	//objdump.str_tab = &objdump.shdr[objdump.elf->e_shstrndx];
	printf("\n%s:     file format %s\n", objdump.file_name, "elf64-x86-64");
	printf("architecture: %s, flags %s\n", objdump.machine_name,
		getflag());
	printf("start address 0x%016lx\n\n", objdump.elf->e_entry);
	const char *const sh_strtab_p = objdump.buf + objdump.str_tab->sh_offset;
	for (i = 0; i < objdump.shnum; ++i) {
	//	printf("%s\n", (char*)(sh_strtab_p + objdump.shd[i].sh_name));
	}
}

int start(void)
{
	if (objdump.fd != -1) {
		fstat(objdump.fd, &objdump.s);
		if(S_ISDIR(objdump.s.st_mode))
			return (dprintf(2, "my_objdump: « %s »: is a folder.\n",
				objdump.file_name) * 0 + 84);
		objdump.buf = mmap(NULL, objdump.s.st_size, PROT_READ, MAP_PRIVATE, objdump.fd,
			0);
		if (objdump.buf != NULL && verif_flag((Elf64_Ehdr *)objdump.buf)) {
			my_objdump();
		} else {
			dprintf(2, "my_objdump: « %s »: not a valid file.\n",
				objdump.file_name);
			return 84;
		}
		close(objdump.fd);
	} else {
		dprintf(2, "my_objdump: « %s »: file not found.\n", objdump.file_name);
		return 84;
	}
}

int main(int ac, char **av)
{
	int i = 1;

	objdump.nb_file = ac;
	if (ac == 1) {
		objdump.fd = open("a.out", O_RDONLY);
		start();
	} else {
		while (i < ac) {
			objdump.file_name = av[i];
			objdump.fd = open(av[i], O_RDONLY);
			if (ac == 2 && start() == 84)
				return 84;
			++i;
		}
	}
	return 0;
}
