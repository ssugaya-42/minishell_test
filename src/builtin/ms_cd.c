/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssugaya <ssugaya@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-03-24 12:22:40 by ssugaya           #+#    #+#             */
/*   Updated: 2026-03-24 12:22:40 by ssugaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static int	print_cd_error(char *message)
{
	return (print_error("minishell", "cd", message));
}

static char	*resolve_cd_target(t_cmd *cmd, t_shell *shell, int *print_path)
{
	if (!cmd->argv[1])
		return (env_get_value(shell->env_list, "HOME"));
	if (cmd->argv[2])
		return (NULL);
	if (ft_strcmp(cmd->argv[1], "-") == 0)
	{
		*print_path = 1;
		return (env_get_value(shell->env_list, "OLDPWD"));
	}
	return (cmd->argv[1]);
}

static int	update_pwd_vars(t_shell *shell, char *oldpwd, char *newpwd)
{
	if (oldpwd && !env_set_value(&shell->env_list, "OLDPWD", oldpwd))
		return (0);
	if (newpwd && !env_set_value(&shell->env_list, "PWD", newpwd))
		return (0);
	if (!shell_refresh_envp(shell))
		return (0);
	return (1);
}

static int	handle_cd_errors(t_cmd *cmd)
{
	if (cmd->argv[2])
		return (print_cd_error("too many arguments"));
	if (!cmd->argv[1])
		return (print_cd_error("HOME not set"));
	return (print_cd_error("OLDPWD not set"));
}

int	ms_cd(t_cmd *cmd, t_shell *shell)
{
	char	*path;
	char	*oldpwd;
	char	*newpwd;
	int		print_path;

	print_path = 0;
	path = resolve_cd_target(cmd, shell, &print_path);
	if (!path)
		return (handle_cd_errors(cmd));
	if (print_path)
		printf("%s\n", path);
	oldpwd = getcwd(NULL, 0);
	if (chdir(path) == -1)
	{
		free(oldpwd);
		return (print_error("minishell", path, strerror(errno)));
	}
	newpwd = getcwd(NULL, 0);
	if (!newpwd)
	{
		free(oldpwd);
		return (print_cd_error(strerror(errno)));
	}
	if (!update_pwd_vars(shell, oldpwd, newpwd))
	{
		free(oldpwd);
		free(newpwd);
		return (1);
	}
	free(oldpwd);
	free(newpwd);
	return (0);
}