/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssugaya <ssugaya@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-03-18 05:22:11 by ssugaya           #+#    #+#             */
/*   Updated: 2026-03-18 05:22:11 by ssugaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int	check_pipe_syntax(t_token *tokens)
{
	if (!tokens)
		return (1);
	if (tokens->type == TOKEN_PIPE)
	{
		print_syntax_error(tokens->value);
		return (0);
	}
	while (tokens)
	{
		if (tokens->type == TOKEN_PIPE)
		{
			if (!tokens->next)
			{
				print_syntax_error(NULL);
				return (0);
			}
			if (tokens->next->type == TOKEN_PIPE)
			{
				print_syntax_error(tokens->next->value);
				return (0);
			}
		}
		tokens = tokens->next;
	}
	return (1);
}