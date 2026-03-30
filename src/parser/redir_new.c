/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_new.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssugaya <ssugaya@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-03-30 07:56:33 by ssugaya           #+#    #+#             */
/*   Updated: 2026-03-30 07:56:33 by ssugaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

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