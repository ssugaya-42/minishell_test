/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssugaya <ssugaya@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-03-20 10:52:32 by ssugaya           #+#    #+#             */
/*   Updated: 2026-03-20 10:52:32 by ssugaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

t_redir	*redir_new(t_token_type type, char *file, t_quote_type file_quote)
{
	t_redir	*new_redir;

	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return (NULL);
	new_redir->type = type;
	new_redir->file = ft_strdup(file);
	if (!new_redir->file)
	{
		free(new_redir);
		return (NULL);
	}
	new_redir->file_quote = file_quote;
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
	t_redir	*new_redir;

	while (tokens && tokens->type != TOKEN_PIPE)
	{
		if (is_redir_token(tokens->type) && tokens->next)
		{
			new_redir = redir_new(tokens->type,
					tokens->next->value, tokens->next->quote);
			if (!new_redir)
				return (0);
			redir_add_back(&cmd->redirs, new_redir);
			tokens = tokens->next;
		}
		tokens = tokens->next;
	}
	return (1);
}