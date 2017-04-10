CC			= gcc
CNAME		=	request-$(shell uname -p)-$(shell uname -r).a
CFLAGS	=	-std=c11 -Wall -Werror -Wextra -Wpedantic
SRC     = add_callback.c		\
					buff_to_request.c \
					btorq.c						\
					data_to_buffer.c	\
					delete_client.c		\
					delete_request.c	\
					flush.c						\
					get_callback.c		\
					recv_request.c		\
					recv_rq.c					\
					rq_client.c				\
					send_request.c		\
					send_rq.c

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
