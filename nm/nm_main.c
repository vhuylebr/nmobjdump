/*
** EPITECH PROJECT, 2018
** nm
** File description:
** nm
*/

# define ELF_RELOC_ERR -1
# define DO_386_64(S, A)	((S) + (A))
# define DO_386_PC64(S, A, P)	((S) + (A) - (P))

# include <fcntl.h>
# include <stdio.h>
# include <elf.h>
# include <sys/mman.h>
# include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

static int elf_do_reloc(Elf64_Ehdr *hdr, Elf64_Rel *rel, Elf64_Shdr *reltab);
static int elf_load_stage1(Elf64_Ehdr *hdr);
static int elf_load_stage2(Elf64_Ehdr *hdr);
static inline Elf64_Shdr *elf_sheader(Elf64_Ehdr *hdr);
static inline Elf64_Shdr *elf_section(Elf64_Ehdr *hdr, int idx);

int verif_flag(Elf64_Ehdr *elf)
{
	//printf ("E=%c L=%c F=%c\n", elf->e_ident[1], elf->e_ident[2], elf->e_ident[3]);
	if (elf->e_ident[1] == 'E' && elf->e_ident[2] == 'L'
		  && elf->e_ident[3] == 'F')
		return 1;
	return 0;

}

char            print_type(Elf64_Sym sym, Elf64_Shdr *shdr)
{
	char  c;

	if (ELF64_ST_BIND(sym.st_info) == STB_GNU_UNIQUE)
		c = 'u';
	else if (ELF64_ST_BIND(sym.st_info) == STB_WEAK) {
		c = 'W';
		if (sym.st_shndx == SHN_UNDEF)
			c = 'w';
	}
	else if (ELF64_ST_BIND(sym.st_info) == STB_WEAK && ELF64_ST_TYPE(sym.st_info) == STT_OBJECT) {
		c = 'V';
		if (sym.st_shndx == SHN_UNDEF)
			c = 'v';
	}
	else if (sym.st_shndx == SHN_UNDEF)
		c = 'U';
	else if (sym.st_shndx == SHN_ABS)
		c = 'A';
	else if (sym.st_shndx == SHN_COMMON)
		c = 'C';
	else if (shdr[sym.st_shndx].sh_type == SHT_NOBITS
		&& shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
		c = 'B';
	else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS
			&& shdr[sym.st_shndx].sh_flags == SHF_ALLOC)
		c = 'R';
	else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS
			&& shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
		c = 'D';
	else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS
			&& shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_EXECINSTR))
		c = 'T';
	else if (shdr[sym.st_shndx].sh_type == SHT_INIT_ARRAY || shdr[sym.st_shndx].sh_type == SHT_FINI_ARRAY)
		return 't';
	else if (shdr[sym.st_shndx].sh_type == SHT_DYNAMIC)
		c = 'D';
	else
		c = '?';
	if (ELF64_ST_BIND(sym.st_info) == STB_LOCAL && c != '?')
		c += 32;
	return c;
}

int main (int ac , char ** av )
{
	int fd;
	void *buf;
	struct stat s;
	Elf64_Ehdr *elf;
	Elf64_Shdr *shd;

	if (ac == 1)
		return 84;
	fd = open (av[1], O_RDONLY);
	if ( fd != -1) {
		fstat (fd, &s);
		buf = mmap( NULL , s.st_size , PROT_READ , MAP_PRIVATE , fd , 0) ;
		if (buf != NULL) {
			elf = buf;
			if (!verif_flag(elf))
				return 84;
			shd = buf + elf->e_shoff;
			Elf64_Ehdr *ehdr = (Elf64_Ehdr*)buf;
			Elf64_Shdr *shdr = (Elf64_Shdr *)(buf + elf->e_shoff);
			Elf64_Shdr *sh_strtab = &shdr[ehdr->e_shstrndx];
			const char *const sh_strtab_p = buf + sh_strtab->sh_offset;
			int shnum = elf->e_shnum;
			
			Elf64_Sym *symtab;
			int i;
			int y;
			Elf64_Shdr *str_tab;
			for (i = 0; i < shnum; ++i) {
				if (shd[i].sh_type == SHT_STRTAB) {
					str_tab = &shd[i];
				}
			}
			for (i = 0; i < shnum; ++i) {
					//printf("%2d: %4d '%s'\n", i, shdr[i].sh_name, sh_strtab_p + shdr[i].sh_name);
				if (shd[i].sh_type == SHT_SYMTAB) {
					symtab = (Elf64_Sym *)(buf + shd[i].sh_offset);
					for (y = 1; y < shd[i].sh_size / shd[i].sh_entsize; ++y) {
						if (symtab[y].st_info != STT_SECTION && symtab[y].st_info != STT_FILE) {
							if (symtab[y].st_shndx == SHN_UNDEF)
								printf("%18c %s\n", print_type(symtab[y], shdr), (char *)(buf + str_tab->sh_offset + symtab[y].st_name));
							else
								printf("%016x %c %s\n", (unsigned int)symtab[y].st_value,  print_type(symtab[y], shdr), (char *)(buf + str_tab->sh_offset + symtab[y].st_name));
						}
					}
				}
			}
		} else
			perror (" mmap ");
		close(fd);
	} else
		return 84;
	return 0;
}
