/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssugaya <ssugaya@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-03-20 12:34:02 by ssugaya           #+#    #+#             */
/*   Updated: 2026-03-20 12:34:02 by ssugaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include <unistd.h>

static char	*get_env_value(t_env *env_list, char *key)
{
	while (env_list)
	{
		if (ft_strcmp(env_list->key, key) == 0)
			return (env_list->value);
		env_list = env_list->next;
	}
	return (NULL);
}

static char	*join_path(char *dir, char *cmd)
{
	char	*tmp;
	char	*full_path;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	full_path = ft_strjoin(tmp, cmd);
	free(tmp);
	return (full_path);
}

static char	*search_in_paths(char **paths, char *cmd_name)
{
	int		i;
	char	*full_path;

	i = 0;
	while (paths[i])
	{
		full_path = join_path(paths[i], cmd_name);
		if (!full_path)
			return (NULL);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

char	*resolve_command_path(char *cmd_name, t_shell *shell)
{
	char	*path_value;
	char	**paths;
	char	*full_path;

	if (!cmd_name || !shell)
		return (NULL);
	if (ft_strchr(cmd_name, '/'))
	{
		if (access(cmd_name, X_OK) == 0)
			return (ft_strdup(cmd_name));
		return (NULL);
	}
	path_value = get_env_value(shell->env_list, "PATH");
	if (!path_value)
		return (NULL);
	paths = ft_split(path_value, ':');
	if (!paths)
		return (NULL);
	full_path = search_in_paths(paths, cmd_name);
	free_str_array(paths);
	return (full_path);
}
