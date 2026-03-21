/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssugaya <ssugaya@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-03-12 07:50:14 by ssugaya           #+#    #+#             */
/*   Updated: 2026-03-12 07:50:14 by ssugaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	init_shell(t_shell *shell, char **envp)
{
	shell->env_list = env_init(envp);
	shell->envp = NULL;
	shell->last_status = 0;
	shell->stdin_backup = dup(STDIN_FILENO);
	shell->stdout_backup = dup(STDOUT_FILENO);
	shell->last_pid = -1;
	shell->is_interactive = isatty(STDIN_FILENO);
}

