/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssugaya <ssugaya@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-03-25 09:09:28 by ssugaya           #+#    #+#             */
/*   Updated: 2026-03-25 09:09:28 by ssugaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static int	is_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

static int	append_char(char **dst, char c)
{
	char	buf[2];
	char	*new_str;

	buf[0] = c;
	buf[1] = '\0';
	new_str = ft_strjoin(*dst, buf);
	if (!new_str)
		return (0);
	free(*dst);
	*dst = new_str;
	return (1);
}

static int	append_str(char **dst, char *src)
{
	char	*new_str;

	new_str = ft_strjoin(*dst, src);
	if (!new_str)
		return (0);
	free(*dst);
	*dst = new_str;
	return (1);
}

static int	append_var_value(char **result, char *src, int *i, t_shell *shell)
{
	char	*name;
	char	*value;
	char	*status_str;
	int		start;

	if (src[*i + 1] == '?')
	{
		status_str = ft_itoa(shell->last_status);
		if (!status_str)
			return (0);
		if (!append_str(result, status_str))
			return (free(status_str), 0);
		free(status_str);
		*i += 2;
		return (1);
	}
	if (!ft_isalpha(src[*i + 1]) && src[*i + 1] != '_')
		return (append_char(result, src[(*i)++]));
	start = *i + 1;
	while (src[*i + 1] && is_var_char(src[*i + 1]))
		(*i)++;
	name = ft_substr(src, start, *i - start + 1);
	if (!name)
		return (0);
	value = env_get_value(shell->env_list, name);
	free(name);
	(*i)++;
	if (!value)
		return (1);
	return (append_str(result, value));
}

char	*expand_part_value(char *value, t_quote_type quote, t_shell *shell)
{
	char	*result;
	int		i;

	if (!value || !shell)
		return (NULL);
	if (quote == QUOTE_SINGLE)
		return (ft_strdup(value));
	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (value[i])
	{
		if (value[i] == '$')
		{
			if (!append_var_value(&result, value, &i, shell))
				return (free(result), NULL);
		}
		else if (!append_char(&result, value[i++]))
			return (free(result), NULL);
	}
	return (result);
}

char	*expand_word_parts(t_word_part *parts, t_shell *shell)
{
	char	*result;
	char	*expanded;
	char	*new_result;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	while (parts)
	{
		expanded = expand_part_value(parts->value, parts->quote, shell);
		if (!expanded)
			return (free(result), NULL);
		new_result = ft_strjoin(result, expanded);
		free(result);
		free(expanded);
		if (!new_result)
			return (NULL);
		result = new_result;
		parts = parts->next;
	}
	return (result);
}