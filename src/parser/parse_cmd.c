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

static int	count_words(t_token *tokens)
{
	int	count;
	int	skip_next;

	count = 0;
	skip_next = 0;
	while (tokens && tokens->type != TOKEN_PIPE)
	{
		if (skip_next)
		{
			skip_next = 0;
			tokens = tokens->next;
			continue ;
		}
		if (is_redir_token(tokens->type))
		{
			skip_next = 1;
			tokens = tokens->next;
			continue ;
		}
		if (tokens->type == TOKEN_WORD)
			count++;
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
	new_cmd->argv_quote = NULL;
	new_cmd->argc = 0;
	new_cmd->redirs = NULL;
	new_cmd->builtin = BI_NONE;
	new_cmd->next = NULL;
	return (new_cmd);
}

void	cmd_add_back(t_cmd **lst, t_cmd *new_cmd)
{
	t_cmd	*cur;

	if (!lst || !new_cmd)
		return ;
	if (!*lst)
	{
		*lst = new_cmd;
		return ;
	}
	cur = *lst;
	while (cur->next)
		cur = cur->next;
	cur->next = new_cmd;
}

static int	fill_argv(t_cmd *cmd, t_token *tokens)
{
	int	i;
	int	skip_next;

	cmd->argc = count_words(tokens);
	cmd->argv = malloc(sizeof(char *) * (cmd->argc + 1));
	cmd->argv_quote = malloc(sizeof(t_quote_type) * cmd->argc);
	if (!cmd->argv || !cmd->argv_quote)
		return (0);
	i = 0;
	skip_next = 0;
	while (tokens && tokens->type != TOKEN_PIPE)
	{
		if (skip_next)
		{
			skip_next = 0;
			tokens = tokens->next;
			continue ;
		}
		if (is_redir_token(tokens->type))
		{
			skip_next = 1;
			tokens = tokens->next;
			continue ;
		}
		if (tokens->type == TOKEN_WORD)
		{
			cmd->argv[i] = ft_strdup(tokens->value);
			if (!cmd->argv[i])
				return (0);
			cmd->argv_quote[i] = tokens->quote;
			i++;
		}
		tokens = tokens->next;
	}
	cmd->argv[i] = NULL;
	return (1);
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

t_cmd	*parse_tokens(t_token *tokens)
{
	t_cmd	*cmds;
	t_cmd	*new_cmd;
	t_token	*start;

	cmds = NULL;
	while (tokens)
	{
		start = tokens;
		while (tokens && tokens->type != TOKEN_PIPE)
			tokens = tokens->next;
		new_cmd = cmd_new();
		if (!new_cmd)
			return (NULL);
		if (!fill_argv(new_cmd, start))
			return (NULL);
		if (!parse_redirs(new_cmd, start))
			return (NULL);
		if (new_cmd->argv && new_cmd->argv[0])
			new_cmd->builtin = get_builtin_type(new_cmd->argv[0]);
		cmd_add_back(&cmds, new_cmd);
		if (tokens)
			tokens = tokens->next;
	}
	return (cmds);
}