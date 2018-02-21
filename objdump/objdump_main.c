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

typedef struct pc_s {
	int macro;
	char *str;
} pc_t;

static void get_machine_name()
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

int print_shdr(const char *const fname, size_t size) {
  int fd = open(fname, O_RDONLY);
  //char *p = mmap(0, size, PROT_READ, MAP_PRIVATE, fd, 0);
	void *p = mmap(NULL, objdump.s.st_size, PROT_READ, MAP_PRIVATE, objdump.fd,
			0);
  Elf32_Ehdr *ehdr = (Elf32_Ehdr*)p;
  Elf32_Shdr *shdr = (Elf32_Shdr *)(p + ehdr->e_shoff);
  int shnum = objdump.elf->e_shnum;

  Elf32_Shdr *sh_strtab = &shdr[ehdr->e_shstrndx];
  const char *const sh_strtab_p = p + sh_strtab->sh_offset;
int i;
  for (i = 0; i < shnum; ++i) {
    printf("%2d: %4d '%s'\n", i, shdr[i].sh_name,
	   sh_strtab_p + shdr[i].sh_name);
  }
  return 0;
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
	printf("architecture: %s, flags 0x%08x:\n", objdump.machine_name,
		objdump.elf->e_flags);
	printf("start address 0x%016lx\n\n", objdump.elf->e_entry);
	const char *const sh_strtab_p = objdump.buf + objdump.str_tab->sh_offset;
	for (i = 0; i < objdump.shnum; ++i) {
		printf("%i\n", (int)objdump.shd[i].sh_name);
		printf("%s\n", (char*)(sh_strtab_p + objdump.shd[i].sh_name));
	}
}

int start()
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
		//muobjdumpap(objdump.buf, objdump.s.st_size);
		close(objdump.fd);
	} else {
		dprintf(2, "my_objdump: « %s »: file not found.\n", objdump.file_name);
		return 84;
	}
}

int main (int ac, char **av)
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
