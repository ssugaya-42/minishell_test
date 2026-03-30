
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssugaya <ssugaya@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-03-30 08:18:40 by ssugaya           #+#    #+#             */
/*   Updated: 2026-03-30 08:18:40 by ssugaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

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

t_redir	*redir_new(t_token_type type, t_word_part *file_parts)
{
	t_redir	*new_redir;

	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return (NULL);
	new_redir->type = type;
	new_redir->file_parts = file_parts;
	new_redir->file = NULL;
	new_redir->heredoc_fd = -1;
	new_redir->next = NULL;
	return (new_redir);
}

void	redir_add_back(t_redir **lst, t_redir *new_redir)
{
	t_redir	*cur;

	if (!lst || !new_redir)
		return ;
	if (!*lst)
	{
		*lst = new_redir;
		return ;
	}
	cur = *lst;
	while (cur->next)
		cur = cur->next;
	cur->next = new_redir;
}

int	parse_redirs(t_cmd *cmd, t_token *tokens)
{
	t_redir		*new_redir;
	t_word_part	*file_parts;

	while (tokens && tokens->type != TOKEN_PIPE)
	{
		if (is_redir_token(tokens->type) && tokens->next)
		{
			file_parts = word_parts_dup(tokens->next->parts);
			if (!file_parts)
				return (0);
			new_redir = redir_new(tokens->type, file_parts);
			if (!new_redir)
				return (free_word_parts(file_parts), 0);
			redir_add_back(&cmd->redirs, new_redir);
			tokens = tokens->next;
		}
		tokens = tokens->next;
	}
	return (1);
}