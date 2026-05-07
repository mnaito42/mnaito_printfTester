NAME		= mnaito_printfTester

CC			= cc
CFLAGS		= -w -g3

SRC			= test.c
OBJ			= $(SRC:.c=.o)

PRINTF_DIR	= ..
PRINTF_LIB	= $(PRINTF_DIR)/libftprintf.a
PRINTF_INC	= -I$(PRINTF_DIR)

RM			= rm -f

UNAME := $(shell uname -s)

ifeq ($(UNAME), Linux)
	VALGRIND = valgrind --leak-check=full --show-leak-kinds=all
endif

GREEN		= \033[32m
BLUE		= \033[34m
RED			= \033[31m
RESET		= \033[0m

.DEFAULT_GOAL := all

all: banner $(NAME)

banner:
	@printf "$(BLUE)"
	@printf "=====================================\n"
	@printf "         ft_printf tester\n"
	@printf "=====================================\n"
	@printf "$(RESET)"

$(NAME): checklibft $(OBJ)
	@printf "$(GREEN)Linking $(NAME)...$(RESET)\n"
	@$(CC) $(CFLAGS) $(OBJ) $(PRINTF_LIB) -o $(NAME)
	@printf "$(GREEN)Build complete.$(RESET)\n"

%.o: %.c
	@$(CC) $(CFLAGS) $(PRINTF_INC) -c $< -o $@

checklibft:
	@if [ ! -f $(PRINTF_LIB) ]; then \
		printf "$(BLUE)Building libftprintf.a...$(RESET)\n"; \
		make -C $(PRINTF_DIR); \
	fi

run: all
	@./$(NAME) > result.txt
	@FAIL_COUNT=$$(grep -c "❌ NG" result.txt || true); \
	if [ $$FAIL_COUNT -gt 0 ]; then \
		printf "$(RED)%d tests failed ❌$(RESET)\n" $$FAIL_COUNT; \
		grep -B 4 "❌ NG" result.txt; \
	else \
		printf "$(GREEN)All tests passed ✅$(RESET)\n"; \
	fi

show:
	@cat result.txt

valgrind: all
	ifdef VALGRIND
		@$(VALGRIND) ./$(NAME)
	else
		@echo "Valgrind is Linux only"
	endif

re: fclean all

clean:
	@$(RM) $(OBJ)
	@make clean -C $(PRINTF_DIR)

fclean: clean
	@$(RM) $(NAME)

.PHONY: all clean fclean re run valgrind banner checklibft