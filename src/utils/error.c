/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssugaya <ssugaya@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-03-12 20:28:28 by ssugaya           #+#    #+#             */
/*   Updated: 2026-03-12 20:28:28 by ssugaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int	print_syntax_error(char *near_token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	if (near_token)
		ft_putstr_fd(near_token, 2);
	else
		ft_putstr_fd("newline", 2);
	ft_putendl_fd("'", 2);
	return (1);
}

int	print_error(char *prefix, char	*target, char *message)
{
	if (prefix)
	{
		ft_putstr_fd(prefix, 2);
		ft_putstr_fd(": ", 2);
	}
	if (target)
	{
		ft_putstr_fd(target, 2);
		ft_putstr_fd(": ", 2);
	}
	if (message)
		ft_putstr_fd(message, 2);
	ft_putchar_fd('\n', 2);
	return (1);
}

void	fatal_error(char *message)
{
	if (message)
		print_error("minishell", NULL, message);
	exit(EXIT_FAILURE);
}
