/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssugaya <ssugaya@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-03-23 11:45:31 by ssugaya           #+#    #+#             */
/*   Updated: 2026-03-23 11:45:31 by ssugaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static int	export_set_value(t_shell *shell, char *arg)
{
	char	*equal;
	char	*key;

	equal = ft_strchr(arg, '=');
	if (!equal)
		return (0);
	key = ft_substr(arg, 0, equal - arg);
	if (!key)
		return (1);
	if (!env_set_value(&shell->env_list, key, equal + 1))
	{
		free(key);
		return (1);
	}
	free(key);
	return (0);
}

static int	handle_export_arg(t_shell *shell, char *arg)
{
	if (!is_valid_identifier(arg))
		return (print_error("minishell", "export",
				"not a valid identifier"));
	return (export_set_value(shell, arg));
}

int	ms_export(t_cmd *cmd, t_shell *shell)
{
	int	i;
	int	status;

	if (!cmd || !shell || !cmd->argv)
		return (1);
	if (!cmd->argv[1])
		return (print_export_list(shell));
	i = 1;
	status = 0;
	while (cmd->argv[i])
	{
		if (handle_export_arg(shell, cmd->argv[i]) != 0)
			status = 1;
		i++;
	}
	if (!shell_refresh_envp(shell))
		return (1);
	return (status);
}