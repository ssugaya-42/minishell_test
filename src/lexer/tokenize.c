/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssugaya <ssugaya@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-03-17 19:14:35 by ssugaya           #+#    #+#             */
/*   Updated: 2026-03-17 19:14:35 by ssugaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static void	skip_spaces(char *line, int *i)
{
	while (line[*i] == ' ' || line[*i] == '\t')
		(*i)++;
}

static int	add_operator_token(t_token **tokens, char *line, int *i)
{
	char			*value;
	t_token			*new_token;
	t_token_type	type;

	if (line[*i] == '|')
	{
		value = ft_strdup("|");
		type = TOKEN_PIPE;
		(*i)++;
	}
	else if (line[*i] == '<' && line[*i + 1] == '<')
	{
		value = ft_strdup("<<");
		type = TOKEN_HEREDOC;
		(*i) += 2;
	}
	else if (line[*i] == '>' && line[*i + 1] == '>')
	{
		value = ft_strdup(">>");
		type = TOKEN_APPEND;
		(*i) += 2;
	}
	else if (line[*i] == '<')
	{
		value = ft_strdup("<");
		type = TOKEN_REDIR_IN;
		(*i)++;
	}
	else
	{
		value = ft_strdup(">");
		type = TOKEN_REDIR_OUT;
		(*i)++;
	}
	if (!value)
		return (0);
	new_token = token_new(value, type, QUOTE_NONE);
	if (!new_token)
	{
		free(value);
		return (0);
	}
	token_add_back(tokens, new_token);
	return (1);
}

static int	add_word_token(t_token **tokens, char *line, int *i)
{
	int		start;
	char	*value;
	t_token	*new_token;

	start = *i;
	while (line[*i]
		&& line[*i] != ' '
		&& line[*i] != '\t'
		&& line[*i] != '|'
		&& line[*i] != '<'
		&& line[*i] != '>')
		(*i)++;
	value = ft_substr(line, start, *i - start);
	if (!value)
		return (0);
	new_token = token_new(value, TOKEN_WORD, QUOTE_NONE);
	if (!new_token)
	{
		free(value);
		return (0);
	}
	token_add_back(tokens, new_token);
	return (1);
}

t_token	*lexer_tokenize(char *line)
{
	t_token	*tokens;
	int		i;

	tokens = NULL;
	i = 0;
	while (line && line[i])
	{
		skip_spaces(line, &i);
		if (!line[i])
			break ;
		if (line[i] == '|' || line[i] == '<' || line[i] == '>')
		{
			if (!add_operator_token(&tokens, line, &i))
			{
				free_tokens(tokens);
				return (NULL);
			}
		}
		else
		{
			if (!add_word_token(&tokens, line, &i))
			{
				free_tokens(tokens);
				return (NULL);
			}
		}
	}
	return (tokens);
}