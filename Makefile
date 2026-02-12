NAME = codexion
CC = cc
CFLAGS = -Wall -Wextra -Werror
INCLUDES    = -I include
HEADER      = include/codexion.h

SRC_DIR     = src/
DATA_DIR    = data/
UTILS_DIR   = utils/
PARSE_DIR   = parse/

DATA_FILES  = dongle_create.c data_create.c coders_create.c
UTILS_FILES = ft_strcmp.c ft_atol.c dongle_utils.c
PARSE_FILES = parse.c
CORE_FILES  = virtual.c monitoring.c write_status.c

SRC = $(addprefix $(DATA_DIR), $(DATA_FILES)) \
      $(addprefix $(UTILS_DIR), $(UTILS_FILES)) \
      $(addprefix $(PARSE_DIR), $(PARSE_FILES)) \
      $(addprefix $(SRC_DIR), $(CORE_FILES))\
      codexion.c

OBJS = $(SRC:.c=.o)
all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c Makefile $(HEADER)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
