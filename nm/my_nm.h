/*
** EPITECH PROJECT, 2018
** my_nm
** File description:
** my_nm
*/

#ifndef MY_NM_H_
# define MY_NM_H_

# include <fcntl.h>
# include <stdio.h>
# include <elf.h>
# include <sys/mman.h>
# include <sys/stat.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>

typedef struct nmtab_s {
	int value;
	char *name;
	char type;
} nmtab_t;

typedef struct nm_s {
	int fd;
	void *buf;
	nmtab_t *nmtab;
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
}	nm_t;

void get_str_tab(void);
Elf64_Shdr get_section(int macro);
void fill_nmtab();
void aff_nmtab();
void sort_nmtab();
char print_type(Elf64_Sym sym, Elf64_Shdr *shdr);

nm_t nm;

#endif /* !MY_NM_H_ */
