/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssugaya <ssugaya@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-03-21 16:51:33 by ssugaya           #+#    #+#             */
/*   Updated: 2026-03-21 16:51:33 by ssugaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

static int	wait_all_children(pid_t last_pid, t_shell *shell)
{
	int		status;
	pid_t	pid;

	status = 0;
	pid = 0;
	while (1)
	{
		pid = wait(&status);
		if (pid == -1)
			break ;
		if (pid == last_pid)
		{
			if (WIFEXITED(status))
				shell->last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				shell->last_status = 128 + WTERMSIG(status);
		}
	}
	return (shell->last_status);
}

static int	child_exec(t_cmd *cmd, t_shell *shell, int prev_fd, int pipe_fd[2])
{
	if (prev_fd != -1)
	{
		if (dup2(prev_fd, STDIN_FILENO) == -1)
			return (print_error("minishell", "dup2", strerror(errno)));
	}
	if (cmd->next)
	{
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
			return (print_error("minishell", "dup2", strerror(errno)));
	}
	if (prev_fd != -1)
		close(prev_fd);
	if (cmd->next)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
	if (!apply_redirs(cmd->redirs))
		exit(1);
	if (is_builtin(cmd))
		exit(exec_builtin(cmd, shell));
	exec_external_child(cmd, shell);
	
}

static int	fork_pipeline_node(t_cmd *cmd, t_shell *shell, int *prev_fd,
		pid_t *last_pid)
{
	int		pipe_fd[2];
	pid_t	pid;

	pipe_fd[0] = -1;
	pipe_fd[1] = -1;
	if (cmd->next && pipe(pipe_fd) == -1)
		return (print_error("minishell", "pipe", strerror(errno)));
	pid = fork();
	if (pid < 0)
		return (print_error("minishell", "fork", strerror(errno)));
	if (pid == 0)
		child_exec(cmd, shell, *prev_fd, pipe_fd);
	if (*prev_fd != -1)
		close(*prev_fd);
	if (cmd->next)
	{
		close(pipe_fd[1]);
		*prev_fd = pipe_fd[0];
	}
	else
		*prev_fd = -1;
	*last_pid = pid;
	return (1);
}

int	execute_cmds(t_cmd *cmds, t_shell *shell)
{
	if (!cmds)
		return (0);
	if (cmds->next)
		return (execute_pipeline(cmds, shell));
	return (execute_single_cmd(cmds, shell));
}

int	execute_pipeline(t_cmd *cmds, t_shell *shell)
{
	int		prev_fd;
	pid_t	last_pid;

	prev_fd = -1;
	last_pid = -1;
	while (cmds)
	{
		if (!fork_pipeline_node(cmds, shell, &prev_fd, &last_pid))
		{
			if (prev_fd != -1)
				close(prev_fd);
			return (1);
		}
		cmds = cmds->next;
	}
	return (wait_all_children(last_pid, shell));
}