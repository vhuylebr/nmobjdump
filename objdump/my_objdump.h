/*
** EPITECH PROJECT, 2018
** objdump
** File description:
** objdump
*/

#ifndef MY_OBJDUP_H_
# define MY_OBJDUP_H_

# include <ctype.h>
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
	Elf32_Ehdr *elf32;
	Elf32_Sym *symtab32;
	Elf32_Shdr *shd32;
	Elf32_Shdr sect_sym32;
	Elf32_Shdr *str_tab32;
	int nb_file;
	int nb_func;
	int shnum;
	char *file_name;
} objdump_t;

typedef struct pc_s {
	int macro;
	char *str;
} pc_t;

void get_machine_name(void);
void get_str_tab(void);
char *getflag(void);
void print_dec(unsigned int y, char *str, unsigned int max);
void print_end(unsigned int y, int i, char *str);
void print_hex(int i);
void print_data(char *sh_strtab_p);
int is_ok(char *str, int i);
int find(int size, char *str, const char **str2);
int verif_flag(Elf64_Ehdr *elf);
void my_objdump(void);
void my_objdump32(void);

objdump_t objdump;


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

#endif /* !MY_OBJDUP_H_ */
