NAME		=	mnaito_printftester

CC			=	cc
CFLAGS		=	-w
RM			=	rm -rf

SRC = test.c
OBJ = $(SRC:.c=.o)

PRINTF_DIR = ../
PRINTF_LIB = $(PRINTF_DIR)/libftprintf.a
PRINTF_INC = -I$(PRINTF_DIR)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(PRINTF_LIB) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(PRINTF_INC) -c $< -o $@

run: all
	./$(NAME)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all run clean fclean re