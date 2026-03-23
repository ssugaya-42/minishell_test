/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssugaya <ssugaya@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-03-20 12:29:35 by ssugaya           #+#    #+#             */
/*   Updated: 2026-03-20 12:29:35 by ssugaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

static int	wait_child(pid_t pid, t_shell *shell)
{
	int	status;

	status = 0;
	if (waitpid(pid, &status, 0) == -1)
		return (print_error("minishell", "waitpid", strerror(errno)));
	if (WIFEXITED(status))
		shell->last_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->last_status = 128 + WTERMSIG(status);
	return (shell->last_status);
}

int	exec_external(t_cmd *cmd, t_shell *shell)
{
	pid_t	pid;
	char	*cmd_path;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (1);
	cmd_path = resolve_command_path(cmd->argv[0], shell);
	if (!cmd_path)
	{
		shell->last_status = 127;
		return (print_error("minishell", cmd->argv[0], "command not found"));
	}
	pid = fork();
	if (pid < 0)
	{
		free(cmd_path);
		return (print_error("minishell", "fork", strerror(errno)));
	}
	if (pid == 0)
	{
		if (!apply_redirs(cmd->redirs))
			exit(1);
		execve(cmd_path, cmd->argv, shell->envp);
		print_error("minishell", cmd->argv[0], strerror(errno));
		free(cmd_path);
		exit(126);
	}
	free(cmd_path);
	return (wait_child(pid, shell));
}

int	exec_external_child(t_cmd *cmd, t_shell *shell)
{
	char	*cmd_path;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		exit(1);
	cmd_path = resolve_command_path(cmd->argv[0], shell);
	if (!cmd_path)
	{
		print_error("minishell", cmd->argv[0], "command not found");
		exit(127);
	}
	execve(cmd_path, cmd->argv, shell->envp);
	print_error("minishell", cmd->argv[0], strerror(errno));
	free(cmd_path);
	exit(126);
}
