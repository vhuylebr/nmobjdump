
##
## EPITECH PROJECT, 2018
## Makefile
## File description:
##
##

NAMENM	=	my_nm
NAMEOBJ	=	my_objdump

SRCSNM	=	nm/nm_main.c

SRCSOBJ	=	objdump/objdump_main.c

CFLAGS	+=	-W -Wall -Wextra -Werror -I include
#CFLAGS	+=	-I includes

CC	=	gcc

RM	=	rm -f

OBJSNM	=	$(SRCSNM:.c=.o)
OBJSOBJ	=	$(SRCSOBJ:.c=.o)

%.o: %.c
		$(CC) -c -o $@ $< $(LDFLAGS)

all:		nm objdump

nm:		$(NAMENM)

objdump:	$(NAMEOBJ);

$(NAMENM):	$(OBJSNM)
		$(CC) $(OBJSNM) -o $(NAMENM) $(LDFLAGS)

$(NAMEOBJ):	$(OBJSOBJ)
		$(CC) $(OBJSOBJ) -o $(NAMEOBJ) $(LDFLAGS)

clean:
		$(RM) $(OBJSNM)
		$(RM) $(OBJSOBJ)

fclean:		clean
		$(RM) $(NAMENM)
		$(RM) $(NAMEOBJ)

re:		fclean all

.PHONY:		all clean fclean re