/*
** EPITECH PROJECT, 2018
** obj
** File description:
** obj
*/

#include "my_objdump.h"

static char const *bad_section_o[] = {
	".rela.eh_frame",
	".strtab",
	".shstrtab",
	".note.GNU-stack",
	".symtab",
	".data",
	".rela.text",
	".bss",
	".rela.rodata"
};

static char const *bad_section_exec[] = {
	".rela.eh_frame",
	".strtab",
	".shstrtab",
	".note.GNU-stack",
	".symtab",
	".rela.text",
	".bss"
};

static char const *bad_section_so[] = {
	".rela.eh_frame",
	".strtab",
	".shstrtab",
	".note.GNU-stack",
	".symtab",
	".rela.text",
	".bss"
};

int verif_flag(Elf64_Ehdr *elf)
{
	if (elf->e_ident[1] == 'E' && elf->e_ident[2] == 'L'
		  && elf->e_ident[3] == 'F')
		return 1;
	return 0;

}

int is_ok(char *str, int i)
{
	if (objdump.elf->e_type == ET_REL) {
		while (i < 9) {
			if (!strcmp(str, bad_section_o[i]))
				return (0);
			++i;
		}
	} else if (objdump.elf->e_type == ET_EXEC) {
		while (i < 7) {
			if (!strcmp(str, bad_section_exec[i]))
				return (0);
			++i;
		}
	} else {
		while (i < 7) {
			if (!strcmp(str, bad_section_exec[i]))
				return (0);
			++i;
		}
	}
	return (1);
}

void my_objdump(void)
{
	objdump.elf = objdump.buf;
	objdump.shnum = objdump.elf->e_shnum;
	objdump.shd = objdump.buf + objdump.elf->e_shoff;
	get_machine_name();
	get_str_tab();
	printf("\n%s:     file format %s\n", objdump.file_name, "elf64-x86-64");
	printf("architecture: %s, flags %s\n", objdump.machine_name,
		getflag());
	printf("start address 0x%016lx\n\n", objdump.elf->e_entry);
	print_data(objdump.buf + objdump.str_tab->sh_offset);
}

int start(void)
{
	if (objdump.fd != -1) {
		fstat(objdump.fd, &objdump.s);
		if(S_ISDIR(objdump.s.st_mode))
			return (dprintf(2, "my_objdump: « %s »: is a folder.\n",
				objdump.file_name) * 0 + 84);
		objdump.buf = mmap(NULL, objdump.s.st_size, PROT_READ,
			MAP_PRIVATE, objdump.fd, 0);
		if (objdump.buf != NULL &&
			verif_flag((Elf64_Ehdr *)objdump.buf)) {
			my_objdump();
		} else {
			dprintf(2, "my_objdump: « %s »: not a valid file.\n",
				objdump.file_name);
			return 84;
		}
		close(objdump.fd);
	} else
		return (dprintf(2, "my_objdump: « %s »: file not found.\n",
			objdump.file_name) * 0 + 84);
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
