/*
** EPITECH PROJECT, 2018
** objdump
** File description:
** objdump
*/

#ifndef MY_OBJDUP_H_
# define MY_OBJDUP_H_

# include <fcntl.h>
# include <stdio.h>
# include <elf.h>
# include <sys/mman.h>
# include <sys/stat.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>

typedef struct objdump_s {
	int fd;
	void *buf;
	char *machine_name;
	struct stat s;
	Elf64_Ehdr *elf;
	Elf64_Sym *symtab;
	Elf64_Shdr *shd;
	Elf64_Shdr sect_sym;
	Elf64_Shdr *str_tab;
	int nb_file;
	int nb_func;
	int shnum;
	char *file_name;
        int osn;
} objdump_t;


typedef struct pc_s {
	int macro;
	char *str;
} pc_t;

pc_t pc[20] = {
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
objdump_t objdump;

#endif /* !MY_OBJDUP_H_ */
