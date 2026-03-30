/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssugaya <ssugaya@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-03-30 08:17:07 by ssugaya           #+#    #+#             */
/*   Updated: 2026-03-30 08:17:07 by ssugaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static int	count_args(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens && tokens->type != TOKEN_PIPE)
	{
		if (tokens->type == TOKEN_WORD)
			count++;
		else if (is_redir_token(tokens->type) && tokens->next)
			tokens = tokens->next;
		tokens = tokens->next;
	}
	return (count);
}

static t_word_part	*word_parts_dup(t_word_part *src)
{
	t_word_part	*dst;
	t_word_part	*new_part;
	char		*dup_value;

	dst = NULL;
	while (src)
	{
		dup_value = ft_strdup(src->value);
		if (!dup_value)
			return (free_word_parts(dst), NULL);
		new_part = word_part_new(dup_value, src->quote);
		if (!new_part)
			return (free(dup_value), free_word_parts(dst), NULL);
		word_part_add_back(&dst, new_part);
		src = src->next;
	}
	return (dst);
}

static int	fill_args(t_cmd *cmd, t_token *tokens)
{
	int	i;

	i = 0;
	while (tokens && tokens->type != TOKEN_PIPE)
	{
		if (tokens->type == TOKEN_WORD)
		{
			cmd->args[i].parts = word_parts_dup(tokens->parts);
			if (!cmd->args[i].parts)
				return (0);
			cmd->args[i].value = NULL;
			i++;
		}
		else if (is_redir_token(tokens->type) && tokens->next)
			tokens = tokens->next;
		tokens = tokens->next;
	}
	cmd->argc = i;
	return (1);
}

t_cmd	*cmd_new(void)
{
	t_cmd	*new_cmd;

	new_cmd = malloc(sizeof(t_cmd));
	if (!new_cmd)
		return (NULL);
	new_cmd->args = NULL;
	new_cmd->argv = NULL;
	new_cmd->argc = 0;
	new_cmd->redirs = NULL;
	new_cmd->builtin = BI_NONE;
	new_cmd->next = NULL;
	return (new_cmd);
}

static int	init_cmd_args(t_cmd *cmd, t_token *tokens)
{
	int	count;

	count = count_args(tokens);
	cmd->args = ft_calloc(count + 1, sizeof(t_arg));
	if (!cmd->args)
		return (0);
	return (fill_args(cmd, tokens));
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

t_cmd	*parse_tokens(t_token *tokens)
{
	t_cmd	*cmds;
	t_cmd	*new_cmd;

	cmds = NULL;
	while (tokens)
	{
		new_cmd = cmd_new();
		if (!new_cmd)
			return (free_cmds(cmds), NULL);
		if (!init_cmd_args(new_cmd, tokens))
			return (free(new_cmd), free_cmds(cmds), NULL);
		if (!parse_redirs(new_cmd, tokens))
			return (free_cmds(new_cmd), free_cmds(cmds), NULL);
		cmd_add_back(&cmds, new_cmd);
		while (tokens && tokens->type != TOKEN_PIPE)
			tokens = tokens->next;
		if (tokens && tokens->type == TOKEN_PIPE)
			tokens = tokens->next;
	}
	return (cmds);
}