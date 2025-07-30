CC = cc
CFLAGS = -Wall -Wextra -Werror -g 
NAME = philo
SRC = main.c helpers.c init.c routine.c monitor.c print.c
OBJ=$(SRC:%.c=%.o)
%.o: %.c
	${CC} ${CFLAGS} -c $< -o $@
all: $(NAME)
${NAME}: ${OBJ}
	${CC} ${CFLAGS} ${OBJ}  -o $@
clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
