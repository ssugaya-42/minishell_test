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
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (0);
	if (is_builtin(cmd))
		return (exec_builtin(cmd, shell));
	return (exec_external(cmd, shell));
}