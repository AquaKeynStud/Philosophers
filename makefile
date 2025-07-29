.PHONY : all clean fclean re norminette valgrind

NAME := philo

BONUS := philo_bonus

# ╭━━━━━━━━━━━━══════════╕出 ❖ BASICS VARIABLES ❖ 力╒═══════════━━━━━━━━━━━━╮ #

CC				:=	cc

CFLAGS			:= -Wall -Wextra -Werror -MMD

RM				:=	rm	-rf

SHOW_MSG_CLEAN	=	true

MAKEFLAGS		+=	--no-print-directory

# ╰━━━━━━━━━━━━━━━━════════════════╛出 ❖ 力╘════════════════━━━━━━━━━━━━━━━━╯ #

# ╭━━━━━━━━━━━━══════════╕出 ❖ FILE TREE ❖ 力╒═══════════━━━━━━━━━━━━╮ #

# directories
D_INC	=	inc/
D_OBJ	=	.obj/
D_DEP	=	.dep/
D_SRC	=	srcs/
D_BON	=	upgrade/

# file lists
LST_SRC		=	main.c				\
				monitoring.c		\
				philosophers.c		\
				manage_threads.c

LST_BON		=	main_bonus.c		\
				semaphores.c		\
				philos_bonus.c		\
				monitor_bonus.c

LST_REQ		=	time.c				\
				utils.c				\

LST_INC		=	philo.h				\
				philo_bonus.h

INC			=	$(addprefix $(D_INC), $(LST_INC))

OBJ			=	$(addprefix $(D_OBJ), $(notdir $(LST_SRC:.c=.o)))

ROBJ		=	$(addprefix $(D_OBJ), $(notdir $(LST_REQ:.c=.o)))

BOBJ		=	$(addprefix $(D_OBJ), $(notdir $(LST_BON:.c=.o)))

DEPS		=	$(addprefix $(D_DEP), $(notdir $(LST_SRC:.c=.d)))

RDEPS		=	$(addprefix $(D_DEP), $(notdir $(LST_REQ:.c=.d)))

BDEPS		=	$(addprefix $(D_DEP), $(notdir $(LST_BON:.c=.d)))

# ╭━━━━━━━━━━━━══════════╕出 ❖ RULES ❖ 力╒═══════════━━━━━━━━━━━━╮ #

all:	$(NAME)

$(NAME):	$(ROBJ) $(OBJ) $(INC) | $(D_OBJ) $(D_DEP) makefile
	$(CC) $(CFLAGS) $(OBJ) $(ROBJ) -o $(NAME)
	@echo "\e[0;32m$(NAME) program created successfully ! 🧬\e[0m"

bonus:	$(D_OBJ).bonus

$(D_OBJ).bonus: $(ROBJ) $(BOBJ) $(INC) | $(D_OBJ) makefile
	$(CC) $(CFLAGS) $(BOBJ) $(ROBJ) -o $(BONUS)
	@touch $(D_OBJ).bonus
	@echo "\e[0;35m$(NAME) bonus program created successfully ! 🍪\e[0m"

$(D_OBJ):
	@mkdir -p $@

$(D_DEP):
	@mkdir -p $(D_DEP)

vpath %.c $(D_SRC) $(D_BON)

$(D_OBJ)%.o: %.c | $(D_OBJ) $(D_DEP)
	@echo "\e[36mCompiling $@...	\e[0m"
	@$(CC) $(CFLAGS) -I$(D_INC) -c $< -o $@
	@mv $(@:.o=.d) $(D_DEP)

-include $(DEPS)
-include $(RDEPS)
-include $(BDEPS)

clean:
ifeq ($(SHOW_MSG_CLEAN), true)
	@echo "\e[0;36mAll $(NAME) objects have been removed 🧹\e[0m"
endif
	@$(RM) $(D_OBJ) $(D_DEP)

fclean:
	@$(MAKE) -s SHOW_MSG_CLEAN=false clean
	@$(RM) $(NAME)
	@$(RM) $(BONUS)
	@echo "\e[0;34m$(NAME) executable deleted ! 🧼\e[0m"

re:
	@$(MAKE) fclean
	@$(MAKE) all
	@echo "\e[0;32m$(NAME) program recreated successfully ! 🫡\e[0m"

norminette:
	norminette $(D_INC) $(D_SRC) $(D_BON)