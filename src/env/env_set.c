/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_set.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssugaya <ssugaya@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-03-24 12:49:25 by ssugaya           #+#    #+#             */
/*   Updated: 2026-03-24 12:49:25 by ssugaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static t_env	*env_new_node(char *key, char *value)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->key = ft_strdup(key);
	if (!new_node->key)
	{
		free(new_node);
		return (NULL);
	}
	if (value)
	{
		new_node->value = ft_strdup(value);
		if (!new_node->value)
		{
			free(new_node->key);
			free(new_node);
			return (NULL);
		}
	}
	else
		new_node->value = NULL;
	new_node->next = NULL;
	return (new_node);
}

static void	env_add_back(t_env **env, t_env *new_node)
{
	t_env	*cur;

	if (!env || !new_node)
		return ;
	if (!*env)
	{
		*env = new_node;
		return ;
	}
	cur = *env;
	while (cur->next)
		cur = cur->next;
	cur->next = new_node;
}

int	env_set_value(t_env **env, char *key, char *value)
{
	t_env	*cur;
	char	*new_value;

	if (!env || !key)
		return (0);
	cur = *env;
	while (cur)
	{
		if (ft_strcmp(cur->key, key) == 0)
		{
			new_value = NULL;
			if (value)
			{
				new_value = ft_strdup(value);
				if (!new_value)
					return (0);
			}
			free(cur->value);
			cur->value = new_value;
			return (1);
		}
		cur = cur->next;
	}
	cur = env_new_node(key, value);
	if (!cur)
		return (0);
	env_add_back(env, cur);
	return (1);
}