/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exprot_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssugaya <ssugaya@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-03-27 12:46:56 by ssugaya           #+#    #+#             */
/*   Updated: 2026-03-27 12:46:56 by ssugaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int	env_count(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

static char	*make_export_entry(t_env *env)
{
	char	*tmp;
	char	*entry;

	tmp = ft_strjoin("declare -x ", env->key);
	if (!tmp)
		return (NULL);
	entry = ft_strjoin(tmp, "=\"");
	free(tmp);
	if (!entry)
		return (NULL);
	tmp = ft_strjoin(entry, env->value);
	free(entry);
	if (!tmp)
		return (NULL);
	entry = ft_strjoin(tmp, "\"");
	free(tmp);
	return (entry);
}

char	**env_to_export_array(t_env *env)
{
	char	**arr;
	int		i;

	arr = malloc(sizeof(char *) * (env_count(env) + 1));
	if (!arr)
		return (NULL);
	i = 0;
	while (env)
	{
		arr[i] = make_export_entry(env);
		if (!arr[i])
		{
			free_str_array(arr);
			return (NULL);
		}
		i++;
		env = env->next;
	}
	arr[i] = NULL;
	return (arr);
}

void	sort_string_array(char **arr)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (arr[i])
	{
		j = i + 1;
		while (arr[j])
		{
			if (ft_strcmp(arr[i], arr[j]) > 0)
			{
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

int	print_export_list(t_shell *shell)
{
	char	**arr;
	int		i;

	arr = env_to_export_array(shell->env_list);
	if (!arr)
		return (1);
	sort_string_array(arr);
	i = 0;
	while (arr[i])
	{
		ft_putendl_fd(arr[i], STDOUT_FILENO);
		i++;
	}
	free_str_array(arr);
	return (0);
}