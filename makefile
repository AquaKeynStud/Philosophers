.PHONY : all bonus clean fclean re reb rm rmb norminette rmv valgrind helgrind

NAME := philo

BONUS := philo_bonus

# ╭━━━━━━━━━━━━══════════╕出 ❖ BASICS VARIABLES ❖ 力╒═══════════━━━━━━━━━━━━╮ #

CC				:=	cc

CFLAGS = -Wall -Wextra -Werror -MMD

RM				:=	rm	-rf

MKDIR			:=	mkdir -p

SHOW_MSG_CLEAN	=	true

MAKEFLAGS		+=	--no-print-directory

# ╰━━━━━━━━━━━━━━━━════════════════╛出 ❖ 力╘════════════════━━━━━━━━━━━━━━━━╯ #

# ╭━━━━━━━━━━━━══════════╕出 ❖ FILE TREE ❖ 力╒═══════════━━━━━━━━━━━━╮ #

# directories
D_INC	=	inc/
D_SRC	=	srcs/
D_BLD	=	.build/
D_BON	=	upgrade/

D_OBJ	=	$(D_BLD)obj/
D_DEP	=	$(D_BLD)dep/
D_REP	=	$(D_BLD)reports/

# file lists
LST_SRC		=	main.c				\
				forks.c				\
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

VALARGS		:=	$(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))

# ╭━━━━━━━━━━━━══════════╕出 ❖ RULES ❖ 力╒═══════════━━━━━━━━━━━━╮ #

all:	$(NAME)

bonus:	$(BONUS)

both:
	@$(MAKE) all
	@$(MAKE) bonus

$(NAME):	$(ROBJ) $(OBJ) $(INC) | $(D_OBJ) $(D_DEP) makefile
	@$(CC) $(CFLAGS) $(OBJ) $(ROBJ) -o $(NAME)
	@echo "\e[32m\e[1m🎍 $(NAME) program created successfully ! 🎍\e[0m"

$(BONUS):	$(ROBJ) $(BOBJ) $(INC) | $(D_OBJ) $(D_DEP) makefile
	@$(CC) $(CFLAGS) $(BOBJ) $(ROBJ) -o $(BONUS)
	@echo "\e[35m\e[1m🍪 $(BONUS) program created successfully ! 🍪\e[0m"

$(D_BLD):
	@$(MKDIR) $@

$(D_OBJ): $(D_BLD)
	@$(MKDIR) $@

$(D_DEP): $(D_BLD)
	@$(MKDIR) $@

vpath %.c $(D_SRC) $(D_BON)

$(D_OBJ)%.o: %.c | $(D_OBJ) $(D_DEP) makefile
	@echo "\e[36mCompiling $@...	\e[0m"
	@$(CC) $(CFLAGS) -I$(D_INC) -c $< -o $@
	@mv $(@:.o=.d) $(D_DEP)

-include $(DEPS)
-include $(RDEPS)
-include $(BDEPS)

clean:
ifeq ($(SHOW_MSG_CLEAN), true)
	@echo "\e[36m\e[1m🫗 All $(NAME) objects have been removed 🫗\e[0m"
endif
	@$(RM) $(D_BLD)

fclean:
	@$(MAKE) -s SHOW_MSG_CLEAN=false clean
	@$(RM) $(NAME)
	@$(RM) $(BONUS)
	@echo "\e[34m\e[1m🧼 $(NAME) executable deleted ! 🧼\e[0m"

re:
	@$(MAKE) fclean
	@$(MAKE) all

reb:
	@$(MAKE) fclean
	@$(MAKE) bonus

rm:
	@$(RM) $(OBJ) $(DEPS) $(NAME)
	@echo "\e[33mDelete of all instances relative to $(NAME) only 🎇\e[0m"

rmb:
	@$(RM) $(BOBJ) $(BDEPS) $(BONUS)
	@echo "\e[33mDelete of all instances relative to $(BONUS) only 🎆\e[0m"

norminette:
	norminette $(D_INC) $(D_SRC) $(D_BON)

$(D_REP): $(D_BLD)
	@$(MKDIR) $@

VOBJ = .dbg/forks.o .dbg/philosophers.o

.dbg/%.o: .dbg/%.c
	@$(CC) -Wall -Wextra -Werror -I$(D_INC) -fPIC -c $< -o $@

vmake: $(VOBJ)
	@$(RM) $(D_OBJ)fork.o $(D_OBJ)philosophers.o
	@mv $(VOBJ) $(D_OBJ)
	@echo "\e[33m\e[1m🧬 Modification des clocks pour valgrind 🧬\e[0m"
	@$(MAKE) all

rmv:
	@$(RM) $(wildcard $(D_REP)*.log)
	@echo "\e[31m\e[1m🍁 Helgrind/Valgrind reports had been suppressed 🍁\e[0m"

valgrind: $(D_REP)
	@$(MAKE) rmv
	@$(MAKE) vmake
	@echo "\e[34m\e[1m🌊 Generating program and valgrind logs ... 🌊\e[0m"
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=$(D_REP)valgrind-%p.log ./$(VALARGS) > $(D_REP)philo.log

helgrind: $(D_REP)
	@$(MAKE) rmv
	@$(MAKE) vmake
	@echo "\e[34m\e[1m🎐 Generating program and helgrind logs ... 🎐\e[0m"
	@valgrind --tool=helgrind --log-file=$(D_REP)helgrind-%p.log ./$(VALARGS) > $(D_REP)philo.log

%:
	@: