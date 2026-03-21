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

t_redir	*redir_new(t_token_type type, char *file)
{
	t_redir	*new_redir;

	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return (NULL);
	new_redir->type = type;
	new_redir->file = file;
	new_redir->heredoc_fd = -1;
	new_redir->next = NULL;
	return (new_redir);
}

void	redir_add_back(t_redir **lst, t_redir *new_redir)
{
	t_redir	*current;

	if (!lst || !new_redir)
		return ;
	if (!*lst)
	{
		*lst = new_redir;
		return ;
	}
	current = *lst;
	while (current->next)
		current = current->next;
	current->next = new_redir;
}

static int	add_redir_node(t_redir **redirs, t_token *tokens)
{
	char	*file;
	t_redir	*new_redir;

	file = ft_strdup(tokens->next->value);
	if (!file)
		return (0);
	new_redir = redir_new(tokens->type, file);
	if (!new_redir)
	{
		free(file);
		return (0);
	}
	redir_add_back(redirs, new_redir);
	return (1);
}

int	parse_redirs(t_cmd *cmd, t_token *tokens)
{
	while (tokens && tokens->type != TOKEN_PIPE)
	{
		if (is_redir_token(tokens->type))
		{
			if (!tokens->next)
				return (0);
			if (!add_redir_node(&cmd->redirs, tokens))
				return (0);
			tokens = tokens->next;
		}
		tokens = tokens->next;
	}
	return (1);
}
