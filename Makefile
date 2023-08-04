NAME		:= philo
SRCS		:= main.c utils.c forks.c routine.c
INCDIR		:= include/
BINDIR		:= bin/
SRCDIR		:= src/
OBJS		:= $(addprefix $(BINDIR), $(SRCS:.c=.o))
CFLAGS		:= -Wall -Wextra -Werror -lpthread -g
RM			:= rm -f
HEADERS		:= -I ./include
CC			:= gcc

all : $(NAME)

leaks :
	valgrind --leak-check=full ./philo 10 20000 3500 2000 3 100

$(BINDIR)%.o : $(SRCDIR)%.c
	@$(CC) -c $(CFLAGS) -o $@ $^ $(HEADERS)

$(NAME) : $(BINDIR) $(OBJS)
	@echo Philo: Compiling!
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(HEADERS)

$(BINDIR) :
	@mkdir $(BINDIR)

clean:
	@echo Philo: Deleting binaries.
	@rm -r $(BINDIR)

fclean: clean
	@echo Philo: Deleting program.
	@rm -f $(NAME)

re : fclean all