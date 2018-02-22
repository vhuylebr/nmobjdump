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
	char *name_s;

	for (i = 0; i < objdump.shnum; ++i) {
		if (objdump.shd[i].sh_type == SHT_STRTAB) {
			objdump.str_tab = &objdump.shd[i];
			name_s = (char*)objdump.buf + objdump.str_tab->sh_offset
				+ objdump.shd[i].sh_name;
			if (!strcmp(name_s, ".shstrtab") && objdump.osn == 0) {
				objdump.osn = objdump.str_tab->sh_offset;
                                objdump.str_tab = &objdump.shd[i];
				break;
			}
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
                        return ("Core\n");
        }
}

void print_dec(int y, char *str, int max)
{
        int tmp = y;

        if (y < max)
                y = y - 16;
        else if (!(y % 16) && y == max) {
                y = y - 16;
        } else {
                y = y - max % 16 - 1;
                --tmp;
        }
        printf("  ");
        while (y < tmp) {
                if (isprint(str[y]))
                        printf("%c", str[y]);
                else
                        printf(".");
                ++y;
        }
}

char *bad_section_o[] = {
        ".rela.eh_frame",
        ".strtab",
        ".shstrtab",
        ".note.GNU-stack",
        ".symtab",
        ".data",
        ".rela.text",
        ".bss"
};

char *bad_section_exec[] = {
        ".rela.eh_frame",
        ".strtab",
        ".shstrtab",
        ".note.GNU-stack",
        ".symtab",
        ".rela.text",
        ".bss"
};

char *bad_section_so[] = {
        ".rela.eh_frame",
        ".strtab",
        ".shstrtab",
        ".note.GNU-stack",
        ".symtab",
        ".rela.text",
        ".bss"
};

int is_ok(char *str)
{
        int i = 0;

        if (objdump.elf->e_type == ET_REL) {
                while (i < 8) {
                        if (!strcmp(str, bad_section_o[i]))
                                return (0);
                        ++i;
                }
                return (1);
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
}

void my_objdump(void)
{
	int i;
        char *sh_strtab_p = NULL;
        int padding;

	objdump.elf = objdump.buf;
	objdump.shnum = objdump.elf->e_shnum;
	objdump.shd = objdump.buf + objdump.elf->e_shoff;
	get_machine_name();
	get_str_tab();
	printf("\n%s:     file format %s\n", objdump.file_name, "elf64-x86-64");
	printf("architecture: %s, flags %s\n", objdump.machine_name,
		getflag());
	printf("start address 0x%016lx\n\n", objdump.elf->e_entry);
	sh_strtab_p = objdump.buf + objdump.str_tab->sh_offset;
	for (i = 1; i < objdump.shnum; ++i) {
                if (is_ok((char*)(sh_strtab_p + objdump.shd[i].sh_name))) {
		        printf("Contents of section %s:\n", (char*)(sh_strtab_p + objdump.shd[i].sh_name));
                        char *str = (char*)(objdump.buf + objdump.shd[i].sh_offset);
                        int y = 1;
                        while (y <= objdump.shd[i].sh_size) {
                                if (y - 1 == 0)
                                        printf(" %04x ", (unsigned int)objdump.shd[i].sh_addr);
                                printf("%02x", (unsigned char)str[y - 1]);
                                if (!(y % 16) && y == objdump.shd[i].sh_size)
                                        break;
                                if (!(y % 4) && y > 0 && (y % 16))
                                        printf(" ");
                                else if (!(y % 16) && y > 0) {
                                        print_dec(y, str, objdump.shd[i].sh_size);
                                        printf("\n %04x ", (unsigned int)objdump.shd[i].sh_addr + y);
                                }
                                ++y;
                        }
                        if (!(y % 16) && y == objdump.shd[i].sh_size) {
                                padding = 0;
                        } else {
                                padding = (16 - (int)objdump.shd[i].sh_size % 16) * 2;
                                printf("%*s", padding + padding / 9, " ");
                        }
                        print_dec(y, str, objdump.shd[i].sh_size);
                        printf("\n");
                }
	}
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
