/*
** EPITECH PROJECT, 2018
** obj
** File description:
** obj
*/

#include "my_objdump.h"

int verif_flag(Elf64_Ehdr *elf)
{
	//printf ("E=%c L=%c F=%c\n", elf->e_ident[1], elf->e_ident[2], elf->e_ident[3]);
	if (elf->e_ident[1] == 'E' && elf->e_ident[2] == 'L'
		  && elf->e_ident[3] == 'F')
		return 1;
	return 0;

}

int start()
{
	if (objdump.fd != -1) {
		fstat(objdump.fd, &objdump.s);
		objdump.buf = mmap(NULL, objdump.s.st_size, PROT_READ, MAP_PRIVATE, objdump.fd,
			0);
		if (objdump.buf != NULL && verif_flag((Elf64_Ehdr *)objdump.buf)) {
			//my_objdump();
                        printf("obj\n");
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
