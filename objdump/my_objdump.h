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

void get_machine_name(void);
void get_str_tab(void);
char *getflag(void);
void print_dec(int y, char *str, int max);
void print_end(int y, int i, char *str);
void print_hex(int i);
int print_data(char *sh_strtab_p);

objdump_t objdump;

#endif /* !MY_OBJDUP_H_ */
