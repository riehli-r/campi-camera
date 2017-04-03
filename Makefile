CC 			= gcc
CNAME 	= campi-camera
CFLAGS 	= -Wall -Werror -Wpedantic -Wextra
CLIBS		= -ljpeg

SRCDIR	= src
OBJDIR	= obj
INCDIR	= inc
TARDIR 	= bin

SRC			= $(wildcard $(SRCDIR)/*.c)
INC			=	$(INCDIR)/webcam.h
OBJ 		= $(patsubst %.c, $(OBJDIR)/%.o, $(SRC))

RM 			= rm -rf

all: 			$(CNAME)

$(OBJDIR)/%.o: %.c
	  			+@[ -d $(OBJDIR) ] || mkdir $(OBJDIR)
					+@[ -d $(OBJDIR)/$(SRCDIR) ] || mkdir $(OBJDIR)/$(SRCDIR)
					$(CC) -o $@ -c $< $(CFLAGS) $(CLIBS)

cp_inc:
					+@[ -d $(SRCDIR)/campi ] || mkdir $(SRCDIR)/campi
					@cp $(INCDIR)/*.h  $(SRCDIR)/campi/

rm_inc:
					@$(RM) src/campi/

$(CNAME): cp_inc $(OBJ) rm_inc
					+@ [ -d $(TARDIR) ] || mkdir $(TARDIR)
					$(CC) $(OBJ) $(CFLAGS) $(CLIBS) -o $(TARDIR)/$(CNAME)

clean:
					@$(RM) $(OBJDIR)

fclean: 	clean rm_inc
					@$(RM) $(TARDIR)

re: 			fclean all
