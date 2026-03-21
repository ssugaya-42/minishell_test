/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssugaya <ssugaya@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-03-18 05:22:19 by ssugaya           #+#    #+#             */
/*   Updated: 2026-03-18 05:22:19 by ssugaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int	validate_syntax(t_token *tokens)
{
	if (!tokens)
		return (1);
	if (!check_pipe_syntax(tokens))
		return (0);
	if (!check_redir_syntax(tokens))
		return (0);
	return (1);
}