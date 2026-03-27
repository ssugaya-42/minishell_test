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

static int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

static void	skip_spaces(char *line, int *i)
{
	while (line[*i] == ' ' || line[*i] == '\t')
		(*i)++;
}

static int	add_operator_token(t_token **tokens, char *line, int *i)
{
	if (line[*i] == '|')
		token_add_back(tokens, token_new(ft_strdup("|"),
				TOKEN_PIPE, QUOTE_NONE));
	else if (line[*i] == '<' && line[*i + 1] == '<')
	{
		token_add_back(tokens, token_new(ft_strdup("<<"),
				TOKEN_HEREDOC, QUOTE_NONE));
		(*i)++;
	}
	else if (line[*i] == '>' && line[*i + 1] == '>')
	{
		token_add_back(tokens, token_new(ft_strdup(">>"),
				TOKEN_APPEND, QUOTE_NONE));
		(*i)++;
	}
	else if (line[*i] == '<')
		token_add_back(tokens, token_new(ft_strdup("<"),
				TOKEN_REDIR_IN, QUOTE_NONE));
	else if (line[*i] == '>')
		token_add_back(tokens, token_new(ft_strdup(">"),
				TOKEN_REDIR_OUT, QUOTE_NONE));
	return (1);
}

static int	add_quoted_word_token(t_token **tokens, char *line, int *i)
{
	char			quote_char;
	t_quote_type	quote_type;
	int				start;
	char			*value;

	quote_char = line[*i];
	if (quote_char == '\'')
		quote_type = QUOTE_SINGLE;
	else
		quote_type = QUOTE_DOUBLE;
	(*i)++;
	start = *i;
	while (line[*i] && line[*i] != quote_char)
		(*i)++;
	if (line[*i] != quote_char)
		return (0);
	value = ft_substr(line, start, *i - start);
	if (!value)
		return (0);
	token_add_back(tokens, token_new(value, TOKEN_WORD, quote_type));
	(*i)++;
	return (1);
}

static int	add_word_token(t_token **tokens, char *line, int *i)
{
	int		start;
	char	*value;

	start = *i;
	while (line[*i]
		&& line[*i] != ' '
		&& line[*i] != '\t'
		&& !is_operator(line[*i])
		&& line[*i] != '\''
		&& line[*i] != '"')
		(*i)++;
	value = ft_substr(line, start, *i - start);
	if (!value)
		return (0);
	token_add_back(tokens, token_new(value, TOKEN_WORD, QUOTE_NONE));
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
		if (is_operator(line[i]))
			add_operator_token(&tokens, line, &i);
		else if (line[i] == '\'' || line[i] == '"')
		{
			if (!add_quoted_word_token(&tokens, line, &i))
				return (free_tokens(tokens), NULL);
			continue ;
		}
		else
		{
			if (!add_word_token(&tokens, line, &i))
				return (free_tokens(tokens), NULL);
			continue ;
		}
		i++;
	}
	return (tokens);
}