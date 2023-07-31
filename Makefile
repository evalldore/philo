NAME		:= philo
SRCS		:= main.c utils.c lists.c
INCDIR		:= include/
BINDIR		:= bin/
SRCDIR		:= src/
OBJS		:= $(addprefix $(BINDIR), $(SRCS:.c=.o))
CFLAGS		:= -Wall -Wextra -Werror -lpthread
RM			:= rm -f
HEADERS		:= -I ./include
CC			:= gcc

all : $(NAME)

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