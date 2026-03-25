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

static char	*dup_env_value(const char *value)
{
	if (value)
		return (ft_strdup(value));
	return (ft_strdup(""));
}

int	env_set_value(t_env **env, const char *key, const char *value)
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
			new_value = dup_env_value(value);
			if (!new_value)
				return (0);
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