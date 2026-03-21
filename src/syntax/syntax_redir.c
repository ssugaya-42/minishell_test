/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssugaya <ssugaya@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-03-18 05:19:32 by ssugaya           #+#    #+#             */
/*   Updated: 2026-03-18 05:19:32 by ssugaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int	is_redir_token(t_token_type type)
{
	return (type == TOKEN_REDIR_IN
		|| type == TOKEN_REDIR_OUT
		|| type == TOKEN_APPEND
		|| type == TOKEN_HEREDOC);
}

int	check_redir_syntax(t_token *tokens)
{
	while (tokens)
	{
		if (is_redir_token(tokens->type))
		{
			if (!tokens->next)
			{
				print_syntax_error(NULL);
				return (0);
			}
			if (tokens->next->type != TOKEN_WORD)
			{
				print_syntax_error(tokens->next->value);
				return (0);
			}
		}
		tokens = tokens->next;
	}
	return (1);
}