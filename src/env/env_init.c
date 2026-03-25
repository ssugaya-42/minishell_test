/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssugaya <ssugaya@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-03-13 07:55:52 by ssugaya           #+#    #+#             */
/*   Updated: 2026-03-13 07:55:52 by ssugaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static void	free_env_list_partial(t_env *env_list)
{
	t_env	*next;

	while (env_list)
	{
		next = env_list->next;
		free(env_list->key);
		free(env_list->value);
		free(env_list);
		env_list = next;
	}
}

static t_env	*create_env_node(const char *env_str)
{
	int		i;
	char	*key;
	t_env	*node;

	i = 0;
	while (env_str[i] && env_str[i] != '=')
		i++;
	key = ft_substr(env_str, 0, i);
	if (!key)
		return (NULL);
	if (env_str[i] == '=')
		node = env_new_node(key, env_str + i + 1);
	else
		node = env_new_node(key, "");
	free(key);
	return (node);
}

t_env	*env_init(char **envp)
{
	t_env	*env_list;
	t_env	*new_node;
	int		i;

	env_list = NULL;
	i = 0;
	while (envp && envp[i])
	{
		new_node = create_env_node(envp[i]);
		if (!new_node)
		{
			free_env_list_partial(env_list);
			return (NULL);
		}
		env_add_back(&env_list, new_node);
		i++;
	}
	return (env_list);
}