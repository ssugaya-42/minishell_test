/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssugaya <ssugaya@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-03-30 08:36:23 by ssugaya           #+#    #+#             */
/*   Updated: 2026-03-30 08:36:23 by ssugaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	free_word_parts(t_word_part *parts)
{
	t_word_part	*next;

	while (parts)
	{
		next = parts->next;
		free(parts->value);
		free(parts);
		parts = next;
	}
}

void	free_tokens(t_token *tokens)
{
	t_token	*next;

	while (tokens)
	{
		next = tokens->next;
		free(tokens->value);
		free_word_parts(tokens->parts);
		free(tokens);
		tokens = next;
	}
}

static void	free_args(t_arg *args, int argc)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (i < argc)
	{
		free_word_parts(args[i].parts);
		free(args[i].value);
		i++;
	}
	free(args);
}

void	free_redirs(t_redir *redirs)
{
	t_redir	*next;

	while (redirs)
	{
		next = redirs->next;
		free_word_parts(redirs->file_parts);
		free(redirs->file);
		free(redirs);
		redirs = next;
	}
}

void	free_cmds(t_cmd *cmds)
{
	t_cmd	*next;

	while (cmds)
	{
		next = cmds->next;
		free_args(cmds->args, cmds->argc);
		free_str_array(cmds->argv);
		free_redirs(cmds->redirs);
		free(cmds);
		cmds = next;
	}
}