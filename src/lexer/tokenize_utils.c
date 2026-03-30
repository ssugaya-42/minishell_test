/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssugaya <ssugaya@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-03-30 07:55:15 by ssugaya           #+#    #+#             */
/*   Updated: 2026-03-30 07:55:15 by ssugaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

t_token	*token_new(t_token_type type)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->value = NULL;
	new_token->type = type;
	new_token->parts = NULL;
	new_token->next = NULL;
	return (new_token);
}

t_word_part	*word_part_new(char *value, t_quote_type quote)
{
	t_word_part	*new_part;

	new_part = malloc(sizeof(t_word_part));
	if (!new_part)
		return (NULL);
	new_part->value = value;
	new_part->quote = quote;
	new_part->next = NULL;
	return (new_part);
}

void	word_part_add_back(t_word_part **lst, t_word_part *new_part)
{
	t_word_part	*cur;

	if (!lst || !new_part)
		return ;
	if (!*lst)
	{
		*lst = new_part;
		return ;
	}
	cur = *lst;
	while (cur->next)
		cur = cur->next;
	cur->next = new_part;
}