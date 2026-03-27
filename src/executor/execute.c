/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssugaya <ssugaya@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-03-20 11:52:52 by ssugaya           #+#    #+#             */
/*   Updated: 2026-03-20 11:52:52 by ssugaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int	execute_single_cmd(t_cmd *cmd, t_shell *shell)
{
	int	status;

	if (!cmd || !cmd->argv || !cmd->argv[0])
	{
		shell->last_status = 0;
		return (0);
	}
	if (is_builtin(cmd))
	{
		if (!backup_stdio(shell))
		{
			shell->last_status = 1;
			return (1);
		}
		if (!apply_redirs(cmd->redirs))
		{
			restore_stdio(shell);
			shell->last_status = 1;
			return (1);
		}
		status = exec_builtin(cmd, shell);
		if (!restore_stdio(shell))
		{
			shell->last_status = 1;
			return (1);
		}
		shell->last_status = status;
		return (status);
	}
	status = exec_external(cmd, shell);
	shell->last_status = status;
	return (status);
}