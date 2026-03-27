/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssugaya <ssugaya@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-03-23 11:46:23 by ssugaya           #+#    #+#             */
/*   Updated: 2026-03-23 11:46:23 by ssugaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static int	expand_argv(t_cmd *cmd, t_shell *shell)
{
	int	i;

	if (!cmd->argv)
		return (1);
	i = 0;
	while (cmd->argv[i])
	{
		if (!expand_one_str(&cmd->argv[i], cmd->argv_quote[i], shell))
			return (0);
		i++;
	}
	return (1);
}

static int	expand_redirs(t_redir *redirs, t_shell *shell)
{
	while (redirs)
	{
		if (redirs->file
			&& !expand_one_str(&redirs->file, redirs->file_quote, shell))
			return (0);
		redirs = redirs->next;
	}
	return (1);
}

void	expand_cmds(t_cmd *cmds, t_shell *shell)
{
	while (cmds)
	{
		if (!expand_argv(cmds, shell))
			return ;
		if (!expand_redirs(cmds->redirs, shell))
			return ;
		cmds = cmds->next;
	}
}