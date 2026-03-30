/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssugaya <ssugaya@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-03-30 09:26:38 by ssugaya           #+#    #+#             */
/*   Updated: 2026-03-30 09:26:38 by ssugaya          ###   ########.fr       */
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

static int	append_part_from_range(t_word_part **parts, char *line,
		int start, int end, t_quote_type quote)
{
	char		*value;
	t_word_part	*new_part;

	value = ft_substr(line, start, end - start);
	if (!value)
		return (0);
	new_part = word_part_new(value, quote);
	if (!new_part)
	{
		free(value);
		return (0);
	}
	word_part_add_back(parts, new_part);
	return (1);
}

static int	read_quoted_part(t_word_part **parts, char *line, int *i)
{
	char			quote_char;
	t_quote_type	quote_type;
	int				start;

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
	if (!append_part_from_range(parts, line, start, *i, quote_type))
		return (0);
	(*i)++;
	return (1);
}

static int	read_plain_part(t_word_part **parts, char *line, int *i)
{
	int	start;

	start = *i;
	while (line[*i]
		&& line[*i] != ' '
		&& line[*i] != '\t'
		&& !is_operator(line[*i])
		&& line[*i] != '\''
		&& line[*i] != '"')
	{
		(*i)++;
	}
	if (start == *i)
		return (1);
	if (!append_part_from_range(parts, line, start, *i, QUOTE_NONE))
		return (0);
	return (1);
}

static int	add_operator_token(t_token **tokens, char *line, int *i)
{
	t_token	*new_token;

	if (line[*i] == '|')
		new_token = token_new(TOKEN_PIPE);
	else if (line[*i] == '<' && line[*i + 1] == '<')
	{
		new_token = token_new(TOKEN_HEREDOC);
		(*i)++;
	}
	else if (line[*i] == '>' && line[*i + 1] == '>')
	{
		new_token = token_new(TOKEN_APPEND);
		(*i)++;
	}
	else if (line[*i] == '<')
		new_token = token_new(TOKEN_REDIR_IN);
	else
		new_token = token_new(TOKEN_REDIR_OUT);
	if (!new_token)
		return (0);
	token_add_back(tokens, new_token);
	return (1);
}

static t_token	*read_word_token(char *line, int *i)
{
	t_token		*token;
	t_word_part	*parts;

	token = token_new(TOKEN_WORD);
	if (!token)
		return (NULL);
	parts = NULL;
	while (line[*i]
		&& line[*i] != ' '
		&& line[*i] != '\t'
		&& !is_operator(line[*i]))
	{
		if (line[*i] == '\'' || line[*i] == '"')
		{
			if (!read_quoted_part(&parts, line, i))
				return (free_word_parts(parts), free(token), NULL);
		}
		else
		{
			if (!read_plain_part(&parts, line, i))
				return (free_word_parts(parts), free(token), NULL);
		}
	}
	token->parts = parts;
	return (token);
}

t_token	*lexer_tokenize(char *line)
{
	t_token	*tokens;
	t_token	*new_token;
	int		i;

	tokens = NULL;
	i = 0;
	while (line && line[i])
	{
		skip_spaces(line, &i);
		if (!line[i])
			break ;
		if (is_operator(line[i]))
		{
			if (!add_operator_token(&tokens, line, &i))
				return (free_tokens(tokens), NULL);
			i++;
		}
		else
		{
			new_token = read_word_token(line, &i);
			if (!new_token)
				return (free_tokens(tokens), NULL);
			token_add_back(&tokens, new_token);
		}
	}
	return (tokens);
}