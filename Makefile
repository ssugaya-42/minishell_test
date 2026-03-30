NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror
INCLUDES = -Iincludes -Ilibft/includes

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

SRCS = \
	src/builtin/ms_echo.c \
	src/builtin/ms_cd.c \
	src/builtin/ms_pwd.c \
	src/builtin/ms_env.c \
	src/builtin/ms_export.c \
	src/builtin/ms_export_utils.c \
	src/builtin/ms_unset.c \
	src/builtin/ms_exit.c \
	src/expander/expand.c \
	src/expander/expand_utils.c \
	src/signal/signals.c \
	src/main/main.c \
	src/main/init.c \
	src/main/loop.c \
	src/utils/free.c \
	src/utils/error.c \
	src/env/env_init.c \
	src/env/env_get.c \
	src/env/env_set.c \
	src/env/env_unset.c \
	src/env/env_to_envp.c \
	src/env/free_envp.c \
	src/env/env_node.c \
	src/env/env_sync.c \
	src/lexer/token_append.c \
	src/lexer/tokenize.c \
	src/syntax/syntax_check.c \
	src/syntax/syntax_pipe.c \
	src/syntax/syntax_redir.c \
	src/parser/parse_cmd.c \
	src/parser/parse_redir.c \
	src/executor/execute.c \
	src/executor/execute_cmd.c \
	src/executor/execute_builtin.c \
	src/executor/execute_pipeline.c \
	src/executor/fork_exec.c \
	src/executor/path.c \
	src/executor/redir.c

OBJS = $(SRCS:.c=.o)

READLINE_LIBS = -lreadline

all: $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(READLINE_LIBS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)
	make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re