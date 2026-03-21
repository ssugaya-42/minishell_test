/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssugaya <ssugaya@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-03-18 05:51:03 by ssugaya           #+#    #+#             */
/*   Updated: 2026-03-18 05:51:03 by ssugaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static void	free_partial_argv(char **argv, int count)
{
	while (count > 0)
	{
		count--;
		free(argv[count]);
	}
	free(argv);
}

static int	count_args(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens && tokens->type != TOKEN_PIPE)
	{
		if (tokens->type == TOKEN_WORD)
			count++;
		else if (is_redir_token(tokens->type))
		{
			if (tokens->next)
				tokens = tokens->next;
		}
		tokens = tokens->next;
	}
	return (count);
}

t_cmd	*cmd_new(void)
{
	t_cmd	*new_cmd;

	new_cmd = malloc(sizeof(t_cmd));
	if (!new_cmd)
		return (NULL);
	new_cmd->argv = NULL;
	new_cmd->argc = 0;
	new_cmd->redirs = NULL;
	new_cmd->builtin = BI_NONE;
	new_cmd->next = NULL;
	return (new_cmd);
}

void	cmd_add_back(t_cmd **lst, t_cmd *new_cmd)
{
	t_cmd	*current;

	if (!lst || !new_cmd)
		return ;
	if (!*lst)
	{
		*lst = new_cmd;
		return ;
	}
	current = *lst;
	while (current->next)
		current = current->next;
	current->next = new_cmd;
}

static t_builtin_type	get_builtin_type(char *cmd_name)
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

static int	fill_argv(t_cmd *cmd, t_token *tokens)
{
	int	i;

	cmd->argc = count_args(tokens);
	cmd->argv = malloc(sizeof(char *) * (cmd->argc + 1));
	if (!cmd->argv)
		return (0);
	i = 0;
	while (tokens && tokens->type != TOKEN_PIPE)
	{
		if (tokens->type == TOKEN_WORD)
		{
			cmd->argv[i] = ft_strdup(tokens->value);
			if (!cmd->argv[i])
			{
				free_partial_argv(cmd->argv, i);
				cmd->argv = NULL;
				return (0);
			}
			i++;
		}
		else if (is_redir_token(tokens->type))
		{
			if (tokens->next)
				tokens = tokens->next;
		}
		tokens = tokens->next;
	}
	cmd->argv[i] = NULL;
	return (1);
}

t_cmd	*parse_tokens(t_token *tokens)
{
	t_cmd	*cmds;
	t_cmd	*new_cmd;

	cmds = NULL;
	while (tokens)
	{
		new_cmd = cmd_new();
		if (!new_cmd)
		{
			free_cmds(cmds);
			return (NULL);
		}
		if (!fill_argv(new_cmd, tokens))
		{
			free_cmds(new_cmd);
			free_cmds(cmds);
			return (NULL);
		}
		if (!parse_redirs(new_cmd, tokens))
		{
			free_cmds(new_cmd);
			free_cmds(cmds);
			return (NULL);
		}
		if (new_cmd->argv && new_cmd->argv[0])
			new_cmd->builtin = get_builtin_type(new_cmd->argv[0]);
		cmd_add_back(&cmds, new_cmd);
		while (tokens && tokens->type != TOKEN_PIPE)
			tokens = tokens->next;
		if (tokens && tokens->type == TOKEN_PIPE)
			tokens = tokens->next;
	}
	return (cmds);
}
