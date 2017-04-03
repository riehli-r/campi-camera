CC 			= gcc
CNAME 	= campi-camera
CFLAGS 	= -Wall -Werror -Wpedantic -Wextra -ljpeg
SRC 		= src/main.c							\
					src/camera_capture.c		\
					src/camera_frame.c			\
					src/cmp_color.c					\
					src/cmp_rgb.c						\
					src/dev_tool.c					\
					src/exit_failure.c 			\
					src/get_infos.c					\
					src/get_server_ip.c			\
					src/init_device.c 			\
					src/multi_ioctl.c 			\
					src/open_device.c 			\
					src/save_current_jpeg.c	\
					src/set_color.c					\
					src/start_camera.c 			\
					src/stop_camera.c				\
					src/yuyv_to_rgb.c

OBJ 		= $(SRC:.c=.o)
RM 			= rm -rf

all: 			$(CNAME)

$(CNAME): $(OBJ)
					$(CC) $(OBJ) $(CFLAGS) -o $(CNAME)

clean:
					$(RM) $(OBJ)

fclean: 	clean
					$(RM) $(CNAME)

re: 			fclean all
