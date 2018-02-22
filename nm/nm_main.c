/*
** EPITECH PROJECT, 2018
** nm
** File description:
** nm
*/

#include "my_nm.h"

int verif_flag(Elf64_Ehdr *elf)
{
	if (elf->e_ident[1] == 'E' && elf->e_ident[2] == 'L'
		  && elf->e_ident[3] == 'F')
		return 1;
	return 0;

}

void my_nm()
{
	nm.elf = nm.buf;
	nm.shnum = nm.elf->e_shnum;
	nm.shd = nm.buf + nm.elf->e_shoff;
	get_str_tab();
	nm.sect_sym = get_section(SHT_SYMTAB);
	nm.symtab = (Elf64_Sym *)(nm.buf + nm.sect_sym.sh_offset);
	if (nm.nb_file > 2)
		printf("\n%s:\n", nm.file_name);
	nm.nb_func = nm.sect_sym.sh_size / nm.sect_sym.sh_entsize;
	nm.nmtab = malloc(sizeof(nmtab_t) * nm.nb_func + 1);
	fill_nmtab();
	sort_nmtab();
	aff_nmtab();
}

int start()
{
	if (nm.fd != -1) {
		fstat(nm.fd, &nm.s);

		if(S_ISDIR(nm.s.st_mode))
			return (dprintf(2, "my_nm: « %s »: is a folder.\n",
				nm.file_name) * 0 + 84);
		nm.buf = mmap(NULL, nm.s.st_size, PROT_READ, MAP_PRIVATE, nm.fd,
			0);
		if (nm.buf != NULL && verif_flag((Elf64_Ehdr *)nm.buf)) {
			my_nm();
		} else {
			dprintf(2, "my_nm: « %s »: not a valid file.\n",
				nm.file_name);
			return 84;
		}
		munmap(nm.buf, nm.s.st_size);
		close(nm.fd);
	} else
		return (dprintf(2, "my_nm: « %s »: file not found.\n",
			nm.file_name) * 0 + 84);
	return 0;
}

int main (int ac, char **av)
{
	int i = 1;
	int ret = 0;

	nm.nb_file = ac;
	if (ac == 1) {
		nm.fd = open("a.out", O_RDONLY);
		nm.file_name = "a.out";
		return start();
	} else {
		while (i < ac) {
			nm.file_name = av[i];
			nm.fd = open(av[i], O_RDONLY);
			ret = start();
			if (ret == 84 && ac == 2)
				return 84;
			++i;
		}
	}
	return 0;
}
