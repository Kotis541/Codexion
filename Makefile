NAME=codexion
SRCS = heap.c main.c monitor.c parser.c routine.c scheduler.c utils.c
CC=cc
OBJS = $(SRCS:.c=.o)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME) -lpthread

%.o: %.c
	$(CC) -Wall -Werror -Wextra -c $< -o $@

all:	$(NAME)

clean: 
	rm -f *.o

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re