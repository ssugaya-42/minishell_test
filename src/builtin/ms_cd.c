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

static char	*get_cd_path(t_cmd *cmd, t_shell *shell)
{
	char	*path;

	if (!cmd->argv[1])
	{
		path = env_get_value(shell->env_list, "HOME");
		if (!path)
		{
			print_error("minishell", "cd", "HOME not set");
			return (NULL);
		}
		return (path);
	}
	if (cmd->argv[2])
	{
		print_error("minishell", "cd", "too many arguments");
		return (NULL);
	}
	if (ft_strcmp(cmd->argv[1], "-") == 0)
	{
		path = env_get_value(shell->env_list, "OLDPWD");
		if (!path)
		{
			print_error("minishell", "cd", "OLDPWD not set");
			return (NULL);
		}
		printf("%s\n", path);
		return (path);
	}
	return (cmd->argv[1]);
}

static int	update_env_pwd(t_shell *shell, char *oldpwd, char *newpwd)
{
	if (oldpwd && !env_set_value(&shell->env_list, "OLDPWD", oldpwd))
		return (1);
	if (newpwd && !env_set_value(&shell->env_list, "PWD", newpwd))
		return (1);
	free_envp(shell->envp);
	shell->envp = env_to_envp(shell->env_list);
	if (!shell->envp)
		return (1);
	return (0);
}

int	ms_cd(t_cmd *cmd, t_shell *shell)
{
	char	*path;
	char	*oldpwd;
	char	*newpwd;

	path = get_cd_path(cmd, shell);
	if (!path)
		return (1);
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
		return (print_error("minishell", "cd", strerror(errno)));
	}
	if (update_env_pwd(shell, oldpwd, newpwd))
	{
		free(oldpwd);
		free(newpwd);
		return (1);
	}
	free(oldpwd);
	free(newpwd);
	return (0);
}