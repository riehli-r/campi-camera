SRCDIR	= src
OBJDIR	= obj
INCDIR	= inc
TARDIR 	= bin
DATADIR = data
LIBDIR	=	lib

CC 			= gcc
CNAME 	= campi-camera
CFLAGS 	= -Wall -Werror -Wpedantic -Wextra
CLIBS		= -ljpeg -lpthread
CULIBS	=	$(LIBDIR)/request-$(shell uname -p)-$(shell uname -r).a

SRC			= $(wildcard $(SRCDIR)/*.c)
OBJ 		= $(patsubst %.c, $(OBJDIR)/%.o, $(SRC))

RM 			= rm -rf

all: 			$(CNAME)
					+@[ -d $(DATADIR) ] || mkdir $(DATADIR)

$(OBJDIR)/%.o: %.c
	  			+@[ -d $(OBJDIR) ] || mkdir $(OBJDIR)
					+@[ -d $(OBJDIR)/$(SRCDIR) ] || mkdir $(OBJDIR)/$(SRCDIR)
					$(CC) -o $@ -c $< $(CFLAGS)

cp_inc:
					+@[ -d $(SRCDIR)/campi ] || mkdir $(SRCDIR)/campi
					cp $(INCDIR)/*.h  $(SRCDIR)/campi/

lrequest:
					+@[ -d $(LIBDIR) ] || mkdir $(LIBDIR)
					+@[ -d $(SRCDIR)/campi ] || mkdir $(SRCDIR)/campi
					$(MAKE) -C $(SRCDIR)/request
					mv $(SRCDIR)/request/request-$(shell uname -p)-$(shell uname -r).a $(LIBDIR)/
					cp $(SRCDIR)/request/request.h $(SRCDIR)/campi/

lib:			lrequest


$(CNAME): cp_inc lib $(OBJ)
					+@[ -d $(TARDIR) ] || mkdir $(TARDIR)
					$(CC) $(OBJ) $(CFLAGS) $(CLIBS) $(CULIBS) -o $(TARDIR)/$(CNAME)

clean_lib:
					$(RM) $(LIBDIR)
					$(MAKE) -C $(SRCDIR)/request fclean

clean_data:
					$(RM) $(DATADIR)

clean_inc:
					$(RM) src/campi/

clean:		clean_inc
					$(RM) $(OBJDIR)

fclean: 	clean clean_data clean_lib
					$(RM) $(TARDIR)

re: 			fclean all
