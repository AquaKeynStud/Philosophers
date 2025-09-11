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

# ╭━━━━━━━━━━━━══════════╕出 ❖ RULES ❖ 力╒═══════════━━━━━━━━━━━━╮ #

all:	$(NAME)

bonus:	$(BONUS)

both:
	@$(MAKE) all
	@$(MAKE) bonus

$(NAME):	$(ROBJ) $(OBJ) $(INC) | $(D_OBJ) $(D_DEP) makefile
	@$(CC) $(CFLAGS) $(OBJ) $(ROBJ) -o $(NAME)
	@echo "\e[1;32m🎍 $(NAME) program created successfully ! 🎍\e[0m"

$(BONUS):	$(ROBJ) $(BOBJ) $(INC) | $(D_OBJ) $(D_DEP) makefile
	@$(CC) $(CFLAGS) $(BOBJ) $(ROBJ) -o $(BONUS)
	@echo "\e[1;35m🍪 $(BONUS) program created successfully ! 🍪\e[0m"

$(D_BLD):
	@$(MKDIR) $@

$(D_OBJ): $(D_BLD)
	@$(MKDIR) $@

$(D_DEP): $(D_BLD)
	@$(MKDIR) $@

vpath %.c $(D_SRC) $(D_BON)

$(D_OBJ)%.o: %.c | $(D_OBJ) $(D_DEP) makefile
	@echo "\e[1;34m☄️  Compiling $^ into $@...	\e[0m"
	@$(CC) $(CFLAGS) -g3 -I$(D_INC) -c $< -o $@
	@mv $(@:.o=.d) $(D_DEP)

-include $(DEPS)
-include $(RDEPS)
-include $(BDEPS)

clean:
ifeq ($(SHOW_MSG_CLEAN), true)
	@echo "\e[1;36m🫗 All $(NAME) objects have been removed 🫗\e[0m"
endif
	@$(RM) $(D_BLD)

fclean:
	@$(MAKE) -s SHOW_MSG_CLEAN=false clean
	@$(RM) $(NAME)
	@$(RM) $(BONUS)
	@echo "\e[1;34m🧼 $(NAME) executable deleted ! 🧼\e[0m"

re:
	@$(MAKE) fclean
	@$(MAKE) all

reb:
	@$(MAKE) fclean
	@$(MAKE) bonus

rm:
	@$(RM) $(OBJ) $(DEPS) $(NAME)
	@echo "\e[1;33mDelete of all instances relative to $(NAME) only 🎇\e[0m"

rmb:
	@$(RM) $(BOBJ) $(BDEPS) $(BONUS)
	@echo "\e[1;33mDelete of all instances relative to $(BONUS) only 🎆\e[0m"

norminette:
	norminette $(D_INC) $(D_SRC) $(D_BON)



# DEBUGGING
PARAMS		:=	$(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))

$(D_REP): $(D_BLD)
	@$(MKDIR) $@

VOBJ = .dbg/forks.o .dbg/philosophers.o

.dbg/%.o: .dbg/%.c
	@$(CC) -Wall -Wextra -Werror -I$(D_INC) -fPIC -c $< -o $@

$(D_REP)wrap.so: .dbg/wrap.c | $(D_REP)
	@gcc -fPIC -shared -o $(D_REP)wrap.so .dbg/wrap.c -ldl -lpthread
	@echo "\e[1;36m🐳 Wrapper created successfully 🐳\e[0m"

def: .dbg/wrap.c | $(D_REP)
	@gcc -DBREAK=$(firstword $(filter-out $@,$(MAKECMDGOALS))) -fPIC -shared -o $(D_REP)wrap.so .dbg/wrap.c -ldl -lpthread
	@echo "\e[1;36m🐳 Wrapper created successfully (mutex limit set to $(firstword $(filter-out $@,$(MAKECMDGOALS)))) 🐳\e[0m"

vmake: $(VOBJ) | $(D_OBJ)
	@$(RM) $(D_OBJ)fork.o $(D_OBJ)philosophers.o
	@mv $(VOBJ) $(D_OBJ)
	@echo "\e[1;33m🧬 Modification of clocks for valgrind 🧬\e[0m"
	@$(MAKE) all

%:
	@:

valgrind: $(D_REP)
	@if ! echo "$(word 2,$(MAKECMDGOALS))" | grep -Eq '^(philo|philo_bonus)$$'; then	\
		echo "\e[1;31m🍁 - Error: Missing program name before parameters - 🍁\e[0m";	\
		exit 1;	\
	fi
	@$(MAKE) rmv
	@$(MAKE) vmake
	@echo "\e[1;34m🌊 Generating program and valgrind logs ... 🌊\e[0m"
	@valgrind						\
	--leak-check=full					\
	--track-origins=yes					\
	--trace-children=yes				\
	--show-leak-kinds=all				\
	--log-file=$(D_REP)valgrind-%p.log	\
	./$(PARAMS) > $(D_REP)philo.log

helgrind: $(D_REP)
	@if ! echo "$(word 2,$(MAKECMDGOALS))" | grep -Eq '^(philo|philo_bonus)$$'; then	\
		echo "\e[1;31m🍁 - Error: Missing program name before parameters - 🍁\e[0m";	\
		exit 1;	\
	fi
	@$(MAKE) rmv
	@$(MAKE) vmake
	@echo "\e[1;34m🎐 Generating program and helgrind logs ... 🎐\e[0m"
	@valgrind						\
	--tool=helgrind						\
	--log-file=$(D_REP)helgrind-%p.log	\
	./$(PARAMS) > $(D_REP)philo.log

break: $(D_REP)wrap.so
	@$(MAKE) all
	@ARGS="$(filter-out $@,$(MAKECMDGOALS))"; \
	LD_PRELOAD=$(D_REP)wrap.so	\
	valgrind --leak-check=full	\
	--show-leak-kinds=all		\
	--track-origins=yes			\
	./philo $$ARGS

rmv:
	@$(RM) $(wildcard $(D_REP)*.log)
	@echo "\e[1;31m🍁 Helgrind/Valgrind reports had been suppressed 🍁\e[0m"

rmw:
	@$(MAKE) -C .. fclean
	@$(RM) .dbg/wrap.so
	@echo "\e[1;31m🍂 Wrapper et programme supprimés 🍂\e[0m"
