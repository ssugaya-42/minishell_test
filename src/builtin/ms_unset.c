/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssugaya <ssugaya@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-03-23 11:45:42 by ssugaya           #+#    #+#             */
/*   Updated: 2026-03-23 11:45:42 by ssugaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

#include "shell.h"

static int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static int	handle_unset_arg(t_shell *shell, char *arg)
{
	if (!is_valid_identifier(arg))
		return (print_error("minishell", "unset", "not a valid identifier"));
	if (!env_unset_value(&shell->env_list, arg))
		return (1);
	return (0);
}

int	ms_unset(t_cmd *cmd, t_shell *shell)
{
	int	i;
	int	status;

	if (!cmd || !shell || !cmd->argv)
		return (1);
	i = 1;
	status = 0;
	while (cmd->argv[i])
	{
		if (handle_unset_arg(shell, cmd->argv[i]) != 0)
			status = 1;
		i++;
	}
	if (!shell_refresh_envp(shell))
		return (1);
	return (status);
}
