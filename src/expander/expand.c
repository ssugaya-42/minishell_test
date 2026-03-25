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

static int	expand_argv(char **argv, t_shell *shell)
{
	int	i;

	if (!argv)
		return (1);
	i = 0;
	while (argv[i])
	{
		if (!expand_one_str(&argv[i], shell))
			return (0);
		i++;
	}
	return (1);
}

static int	expand_redirs(t_redir *redirs, t_shell *shell)
{
	while (redirs)
	{
		if (redirs->file && !expand_one_str(&redirs->file, shell))
			return (0);
		redirs = redirs->next;
	}
	return (1);
}

static int	expand_cmd(t_cmd *cmd, t_shell *shell)
{
	if (!expand_argv(cmd->argv, shell))
		return (0);
	if (!expand_redirs(cmd->redirs, shell))
		return (0);
	return (1);
}

void	expand_cmds(t_cmd *cmds, t_shell *shell)
{
	while (cmds)
	{
		if (!expand_cmd(cmds, shell))
			return ;
		cmds = cmds->next;
	}
}