/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssugaya <ssugaya@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-03-21 15:42:36 by ssugaya           #+#    #+#             */
/*   Updated: 2026-03-21 15:42:36 by ssugaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int	exec_builtin(t_cmd *cmd, t_shell *shell)
{
	if (!cmd)
		return (1);
	if (cmd->builtin == BI_ECHO)
		return (ms_echo(cmd));
	if (cmd->builtin == BI_CD)
		return (ms_cd(cmd, shell));
	if (cmd->builtin == BI_PWD)
		return (ms_pwd());
	if (cmd->builtin == BI_EXPORT)
		return (ms_export(cmd, shell));
	if (cmd->builtin == BI_UNSET)
		return (ms_unset(cmd, shell));
	if (cmd->builtin == BI_ENV)
		return (ms_env(shell));
	if (cmd->builtin == BI_EXIT)
		return (ms_exit(cmd, shell));
	return (1);
}
