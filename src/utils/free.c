/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssugaya <ssugaya@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-03-30 08:19:22 by ssugaya           #+#    #+#             */
/*   Updated: 2026-03-30 08:19:22 by ssugaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static void	free_args(t_arg *args, int argc)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (i < argc)
	{
		free_word_parts(args[i].parts);
		free(args[i].value);
		i++;
	}
	free(args);
}

void	free_redirs(t_redir *redirs)
{
	t_redir	*next;

	while (redirs)
	{
		next = redirs->next;
		free_word_parts(redirs->file_parts);
		free(redirs->file);
		free(redirs);
		redirs = next;
	}
}

void	free_cmds(t_cmd *cmds)
{
	t_cmd	*next;

	while (cmds)
	{
		next = cmds->next;
		free_args(cmds->args, cmds->argc);
		free_str_array(cmds->argv);
		free_redirs(cmds->redirs);
		free(cmds);
		cmds = next;
	}
}