#ifndef STRUCT_H
# define STRUCT_H

# include "enum.h"
# include <sys/types.h>
# include <stdbool.h>

typedef struct s_word_part
{
	char				*value;
	t_quote_type		quote;
	struct s_word_part	*next;
}	t_word_part;

typedef struct s_token
{
	char				*value;
	t_token_type		type;
	t_word_part			*parts;
	struct s_token		*next;
}	t_token;

typedef struct s_redir
{
	t_token_type		type;
	t_word_part			*file_parts;
	char				*file;
	int					heredoc_fd;
	struct s_redir		*next;
}	t_redir;

typedef struct s_arg
{
	t_word_part	*parts;
	char		*value;
}	t_arg;

typedef struct s_cmd
{
	t_arg				*args;
	char				**argv;
	int					argc;
	t_redir				*redirs;
	t_builtin_type		builtin;
	struct s_cmd		*next;
}	t_cmd;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_shell
{
	t_env	*env_list;
	char	**envp;
	int		last_status;
	int		stdin_backup;
	int		stdout_backup;
	pid_t	last_pid;
	int		is_interactive;
}	t_shell;

#endif