SRCDIR	= src
OBJDIR	= obj
INCDIR	= inc
TARDIR 	= bin
DATADIR = data
LIBDIR	=	lib

CC 			= gcc
CNAME 	= campi-camera
CFLAGS 	= -std=c11 -Wall -Werror -Wpedantic -Wextra
CLIBS		= -ljpeg -lpthread
CULIBS	=	$(LIBDIR)/request-$(shell uname -p)-$(shell uname -r).a

SRC			= $(wildcard $(SRCDIR)/*.c)
OBJ 		= $(patsubst %.c, $(OBJDIR)/%.o, $(SRC))

RM 			= rm -rf

all: 			$(CNAME)

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

clean_inc:
					$(RM) src/campi/

clean:		clean_inc
					$(RM) $(OBJDIR)
					$(RM) $(SRCDIR)/request/*.o

fclean: 	clean clean_lib
					$(RM) $(TARDIR)

re: 			fclean all


# ======================== INSTALL ======================== #
install:
					adduser campie_camera --disabled-password --disabled-login --gecos d
					mkdir /home/campie_camera/photos
					chmod 777 /home/campie_camera/photos
					chown campie_camera /home/campie_camera/photos
					mkdir /home/campie_camera/$(DATADIR)
					chmod 777 /home/campie_camera/$(DATADIR)
					chown campie_camera /home/campie_camera/$(DATADIR)
					cp bin/campi-camera /home/campie_camera/
					cp campi-camera.service /etc/systemd/system/campi-camera.service
					systemctl daemon-reload
					systemctl enable campi-camera
					systemctl daemon-reload

uninstall:
					service campi-camera stop
					systemctl disable campi-camera
					rm /etc/systemd/system/campi-camera.service
				 	userdel campie_camera
					rm -r /home/campie_camera
