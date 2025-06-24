CC = cc
CFLAGS = -Wall -Wextra -Werror -g
NAME = philo
SRC = main.c helpers.c
OBJ=$(SRC:%.c=%.o)
%.o: %.c
	${CC} ${CFLAGS} -c $< -o $@
${NAME}: ${OBJ}
	${CC} ${CFLAGS} ${OBJ} -pthread -o $@
clean:
	rm -rf $(OBJ) $(NAME)
