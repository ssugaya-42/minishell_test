#ifndef SHELL_H
# define SHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <fcntl.h>
# include <signal.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "enum.h"
# include "../libft/libft.h"
# include "struct.h"

/*
** main
*/
void	init_shell(t_shell *shell, char **envp);
void	shell_loop(t_shell *shell);

/*
** lexer
*/
t_token	*lexer_tokenize(char *line);
t_token	*token_new(char *value, t_token_type type, t_quote_type quote);
void	token_add_back(t_token **lst, t_token *new_token);

/*
** syntax
*/
int		validate_syntax(t_token *tokens);
int		check_pipe_syntax(t_token *tokens);
int		check_redir_syntax(t_token *tokens);
int		is_redir_token(t_token_type type);

/*
** parser
*/
t_cmd	*parse_tokens(t_token *tokens);
t_cmd	*cmd_new(void);
void	cmd_add_back(t_cmd **lst, t_cmd *new_cmd);

/*
** expander
*/
void	expand_cmds(t_cmd *cmds, t_shell *shell);

/*
** executor
*/
int		execute_cmds(t_cmd *cmds, t_shell *shell);
int		execute_single_cmd(t_cmd *cmd, t_shell *shell);
int		execute_pipeline(t_cmd *cmds, t_shell *shell);
int		is_builtin(t_cmd *cmd);

/*
** builtin
*/
int		ms_echo(t_cmd *cmd);
int		ms_cd(t_cmd *cmd, t_shell *shell);
int		ms_pwd(void);
int		ms_export(t_cmd *cmd, t_shell *shell);
int		ms_unset(t_cmd *cmd, t_shell *shell);
int		ms_env(t_shell *shell);
int		ms_exit(t_cmd *cmd, t_shell *shell);

/*
** env
*/
t_env	*env_init(char **envp);
char	*env_get_value(t_env *env, char *key);
int		env_set_value(t_env **env, char *key, char *value);
int		env_unset_value(t_env **env, char *key);
char	**env_to_envp(t_env *env);
void	free_envp(char **envp);

/*
** redir
*/
int		apply_redirs(t_redir *redirs);
int		handle_heredoc(t_redir *redir, t_shell *shell);
void	restore_stdio(t_shell *shell);

/*
** signal
*/
void	set_interactive_signals(void);
void	set_child_signals(void);

/*
** utils / free / error
*/
void	free_tokens(t_token *tokens);
void	free_redirs(t_redir *redirs);
void	free_cmds(t_cmd *cmds);
void	free_shell(t_shell *shell);
void	free_str_array(char **arr);

int		print_syntax_error(char *near_token);
int		print_error(char *prefix, char *target, char *message);
void	fatal_error(char *message);

#endif