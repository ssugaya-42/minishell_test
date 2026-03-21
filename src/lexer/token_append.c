/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_append.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssugaya <ssugaya@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-03-17 09:01:26 by ssugaya           #+#    #+#             */
/*   Updated: 2026-03-17 09:01:26 by ssugaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

t_token	*token_new(char *value, t_token_type type, t_quote_type quote)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->value = value;
	new_token->type = type;
	new_token->quote = quote;
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
