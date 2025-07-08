CC = cc
CFLAGS = -Wall -Wextra -Werror -g
NAME = philo
SRC = main.c helpers.c init.c routine.c monitor.c
OBJ=$(SRC:%.c=%.o)
%.o: %.c
	${CC} ${CFLAGS} -c $< -o $@
${NAME}: ${OBJ}
	${CC} ${CFLAGS} ${OBJ} -pthread -o $@
clean:
	rm -rf $(OBJ) $(NAME)
