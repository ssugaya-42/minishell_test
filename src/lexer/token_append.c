/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_append.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssugaya <ssugaya@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-03-30 09:26:14 by ssugaya           #+#    #+#             */
/*   Updated: 2026-03-30 09:26:14 by ssugaya          ###   ########.fr       */
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

void	token_add_back(t_token **lst, t_token *new_token)
{
	t_token	*current;

	if (!lst || !new_token)
		return ;
	if (!*lst)
	{
		*lst = new_token;
		return ;
	}
	current = *lst;
	while (current->next)
		current = current->next;
	current->next = new_token;
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
	t_word_part	*current;

	if (!lst || !new_part)
		return ;
	if (!*lst)
	{
		*lst = new_part;
		return ;
	}
	current = *lst;
	while (current->next)
		current = current->next;
	current->next = new_part;
}