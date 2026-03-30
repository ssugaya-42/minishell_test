/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_word_parts.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssugaya <ssugaya@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-03-30 07:55:56 by ssugaya           #+#    #+#             */
/*   Updated: 2026-03-30 07:55:56 by ssugaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	free_word_parts(t_word_part *parts)
{
	t_word_part	*next;

	while (parts)
	{
		next = parts->next;
		free(parts->value);
		free(parts);
		parts = next;
	}
}