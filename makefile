.PHONY : all clean fclean re norminette valgrind

NAME = philo

# ╭━━━━━━━━━━━━══════════╕出 ❖ BASICS VARIABLES ❖ 力╒═══════════━━━━━━━━━━━━╮ #

CC				:=	cc

CFLAGS			:= -Wall -Wextra -Werror -MMD

RM				:=	rm	-rf

SHOW_MSG_CLEAN	=	true

MAKEFLAGS		+=	--no-print-directory

# ╰━━━━━━━━━━━━━━━━════════════════╛出 ❖ 力╘════════════════━━━━━━━━━━━━━━━━╯ #

# ╭━━━━━━━━━━━━══════════╕出 ❖ FILE TREE ❖ 力╒═══════════━━━━━━━━━━━━╮ #

# directories
D_SRC	=	srcs/
D_INC	=	inc/
D_OBJ	=	.obj/
D_DEP	=	.dep/

# file lists
LST_SRC		=	main.c			\
				time.c			\
				simu.c			\
				utils.c			\
				cleanup.c		\
				monitoring.c	\
				philosophers.c

LST_INC		=	philo.h

INC			=	$(addprefix $(D_INC), $(LST_INC))

OBJ			=	$(addprefix $(D_OBJ), $(notdir $(LST_SRC:.c=.o)))

DEPS		=	$(addprefix $(D_DEP), $(notdir $(LST_SRC:.c=.d)))

# ╭━━━━━━━━━━━━══════════╕出 ❖ RULES ❖ 力╒═══════════━━━━━━━━━━━━╮ #

all:	$(NAME)

$(NAME):	$(OBJ) $(INC) | $(D_OBJ) $(D_DEP) makefile
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@echo "\e[0;32m$(NAME) program created successfully ! 🧬\e[0m"

$(D_OBJ):
	@mkdir -p $@

$(D_DEP):
	@mkdir -p $(D_DEP)

vpath %.c $(D_SRC)

$(D_OBJ)%.o: %.c | $(D_OBJ) $(D_DEP)
	@echo "\e[36mCompiling $@...	\e[0m"
	@$(CC) $(CFLAGS) -g3 -I$(D_INC) -c $< -o $@
	@mv $(@:.o=.d) $(D_DEP)

-include $(DEPS)

clean:
ifeq ($(SHOW_MSG_CLEAN), true)
	@echo "\e[0;36mAll $(NAME) objects have been removed 🧹\e[0m"
endif
	@$(RM) $(D_OBJ) $(D_DEP)

fclean:
	@$(MAKE) -s SHOW_MSG_CLEAN=false clean
	@$(RM) $(NAME)
	@echo "\e[0;34m$(NAME) executable deleted ! 🧼\e[0m"

re:
	@$(MAKE) fclean
	@$(MAKE) all
	@echo "\e[0;32m$(NAME) program recreated successfully ! 🫡\e[0m"

norminette:
	norminette $(D_SRC) $(D_INC)

valgrind: supp_file
	@$(MAKE)
	valgrind								\
		--leak-check=full					\
		--show-leak-kinds=all				\
		--track-origins=yes 				\
		--track-fds=yes						\
		./$(NAME)
	@clear