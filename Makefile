CC = cc
FLAGS = -Wall -Wextra -Werror
SAN = -fsanitize=address -g
LFLAGS = -lreadline

TARGET = minishell
SRCS = ft_atoi.c input_handler.c main.c parser.c parser_ast.c parser_ast_free.c parser_ast_helpers_0.c parser_ast_helpers_1.c parser_stack_helpers.c parser_stack_ops.c strings.c tokenizer.c utilities_0.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(FLAGS) $(SAN) -o $(TARGET) $(OBJS) $(LFLAGS)

%.o: %.c
	$(CC) $(FLAGS) $(SAN) -c $< -o $@ $(LFLAGS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(TARGET)

re: fclean all
