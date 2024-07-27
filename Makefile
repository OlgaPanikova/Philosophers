NAME = philo
SRCDIR = src
OBJDIR = obj
SRCS = $(addprefix $(SRCDIR)/, main.c check.c free.c time.c live.c philo.c)
OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))
CC = cc
RM = rm -rf
CFLAGS = -Wall -Wextra -Werror
HEADER = $(SRCDIR)/philo.h

all: $(NAME) 

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADER) Makefile
	@ mkdir -p $(OBJDIR)
	@ $(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	$(RM) $(OBJS) 
	$(RM) $(OBJDIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re 