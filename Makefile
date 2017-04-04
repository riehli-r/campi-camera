CC 			= gcc
CNAME 	= campi-camera
CFLAGS 	= -Wall -Werror -Wpedantic -Wextra
CLIBS		= -ljpeg -lpthread

SRCDIR	= src
OBJDIR	= obj
INCDIR	= inc
TARDIR 	= bin
DATADIR = data

SRC			= $(wildcard $(SRCDIR)/*.c)
INC			=	$(INCDIR)/webcam.h
OBJ 		= $(patsubst %.c, $(OBJDIR)/%.o, $(SRC))

RM 			= rm -rf

all: 			$(CNAME)
					+@[ -d $(DATADIR) ] || mkdir $(DATADIR)

$(OBJDIR)/%.o: %.c
	  			+@[ -d $(OBJDIR) ] || mkdir $(OBJDIR)
					+@[ -d $(OBJDIR)/$(SRCDIR) ] || mkdir $(OBJDIR)/$(SRCDIR)
					$(CC) -o $@ -c $< $(CFLAGS) $(CLIBS)

cp_inc:
					+@[ -d $(SRCDIR)/campi ] || mkdir $(SRCDIR)/campi
					cp $(INCDIR)/*.h  $(SRCDIR)/campi/


$(CNAME): cp_inc $(OBJ)
					+@ [ -d $(TARDIR) ] || mkdir $(TARDIR)
					$(CC) $(OBJ) $(CFLAGS) $(CLIBS) -o $(TARDIR)/$(CNAME)


clean_data:
					$(RM) $(DATADIR)

clean_inc:
					$(RM) src/campi/

clean:		clean_inc
					$(RM) $(OBJDIR)

fclean: 	clean clean_data
					$(RM) $(TARDIR)

re: 			fclean all
