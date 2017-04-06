CC			= gcc
CNAME		=	request-$(shell uname -p)-$(shell uname -r).a
CFLAGS	=	-Wall -Werror -Wextra -Wpedantic
SRC     = buff_to_request.c \
					delete_request.c	\
					flush.c						\
					recv_request.c		\
					send_request.c

RM			= rm -rf
AR			=	ar r
RAN			=	ranlib
OBJ 		= $(SRC:.c=.o)

all:			$(CNAME)

$(CNAME):	$(OBJ)
					$(AR) $(CNAME) $(OBJ)
					$(RAN) $(CNAME)

clean:
					$(RM) $(OBJ)

fclean:		clean
					$(RM) $(CNAME)

re:				fclean all
