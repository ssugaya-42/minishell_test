/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssugaya <ssugaya@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-03-30 08:35:33 by ssugaya           #+#    #+#             */
/*   Updated: 2026-03-30 08:35:33 by ssugaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static t_builtin_type	get_builtin_type_from_argv0(char *cmd_name)
{
	if (!cmd_name)
		return (BI_NONE);
	if (ft_strcmp(cmd_name, "echo") == 0)
		return (BI_ECHO);
	if (ft_strcmp(cmd_name, "cd") == 0)
		return (BI_CD);
	if (ft_strcmp(cmd_name, "pwd") == 0)
		return (BI_PWD);
	if (ft_strcmp(cmd_name, "export") == 0)
		return (BI_EXPORT);
	if (ft_strcmp(cmd_name, "unset") == 0)
		return (BI_UNSET);
	if (ft_strcmp(cmd_name, "env") == 0)
		return (BI_ENV);
	if (ft_strcmp(cmd_name, "exit") == 0)
		return (BI_EXIT);
	return (BI_NONE);
}

int	build_cmd_argv(t_cmd *cmd)
{
	int	i;

	if (!cmd || !cmd->args || cmd->argc < 0)
		return (0);
	free_str_array(cmd->argv);
	cmd->argv = ft_calloc(cmd->argc + 1, sizeof(char *));
	if (!cmd->argv)
		return (0);
	i = 0;
	while (i < cmd->argc)
	{
		if (!cmd->args[i].value)
			cmd->argv[i] = ft_strdup("");
		else
			cmd->argv[i] = ft_strdup(cmd->args[i].value);
		if (!cmd->argv[i])
			return (0);
		i++;
	}
	cmd->argv[i] = NULL;
	cmd->builtin = get_builtin_type_from_argv0(cmd->argv[0]);
	return (1);
}

int	expand_redirs(t_redir *redirs, t_shell *shell)
{
	char	*expanded;

	while (redirs)
	{
		expanded = expand_word_parts(redirs->file_parts, shell);
		if (!expanded)
			return (0);
		free(redirs->file);
		redirs->file = expanded;
		redirs = redirs->next;
	}
	return (1);
}

static int	expand_args(t_cmd *cmd, t_shell *shell)
{
	int		i;
	char	*expanded;

	i = 0;
	while (i < cmd->argc)
	{
		expanded = expand_word_parts(cmd->args[i].parts, shell);
		if (!expanded)
			return (0);
		free(cmd->args[i].value);
		cmd->args[i].value = expanded;
		i++;
	}
	return (1);
}

int	expand_cmds(t_cmd *cmds, t_shell *shell)
{
	while (cmds)
	{
		if (!expand_args(cmds, shell))
			return (0);
		if (!expand_redirs(cmds->redirs, shell))
			return (0);
		if (!build_cmd_argv(cmds))
			return (0);
		cmds = cmds->next;
	}
	return (1);
}