/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssugaya <ssugaya@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-03-30 08:34:58 by ssugaya           #+#    #+#             */
/*   Updated: 2026-03-30 08:34:58 by ssugaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static void	process_line(t_shell *shell, char *line)
{
	t_token	*tokens;
	t_cmd	*cmds;

	if (!line || line[0] == '\0')
		return ;
	add_history(line);
	tokens = lexer_tokenize(line);
	if (!tokens)
		return ;
	if (validate_syntax(tokens) == 0)
	{
		free_tokens(tokens);
		return ;
	}
	cmds = parse_tokens(tokens);
	free_tokens(tokens);
	if (!cmds)
		return ;
	if (!expand_cmds(cmds, shell))
	{
		shell->last_status = 1;
		free_cmds(cmds);
		return ;
	}
	execute_cmds(cmds, shell);
	free_cmds(cmds);
}

void	shell_loop(t_shell *shell)
{
	char	*line;

	while (1)
	{
		set_interactive_signals();
		line = readline("minishell$ ");
		if (!line)
		{
			write(1, "exit\n", 5);
			break ;
		}
		process_line(shell, line);
		free(line);
	}
}