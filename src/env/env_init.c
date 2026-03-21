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

static t_env	*env_new(char *key, char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = key;
	node->value = value;
	node->next = NULL;
	return (node);
}

static void	env_add_back(t_env **env_list, t_env *new_node)
{
	t_env	*current;

	if (!env_list || !new_node)
		return ;
	if (!*env_list)
	{
		*env_list = new_node;
		return ;
	}
	current = *env_list;
	while (current->next)
		current = current->next;
	current->next = new_node;
}

static t_env	*create_env_node(char *env_str)
{
	int		i;
	char	*key;
	char	*value;
	t_env	*node;

	i = 0;
	while (env_str[i] && env_str[i] != '=')
		i++;
	key = ft_substr(env_str, 0, i);
	if (!key)
		return (NULL);
	if (env_str[i] == '=')
		value = ft_strdup(env_str + i + 1);
	else
		value = ft_strdup("");
	if (!value)
	{
		free(key);
		return (NULL);
	}
	node = env_new(key, value);
	if (!node)
	{
		free(key);
		free(value);
		return (NULL);
	}
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
			while (env_list)
			{
				new_node = env_list->next;
				free(env_list->key);
				free(env_list->value);
				free(env_list);
				env_list = new_node;
			}
			return (NULL);
		}
		env_add_back(&env_list, new_node);
		i++;
	}
	return (env_list);
}
