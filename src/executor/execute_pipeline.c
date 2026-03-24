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

static void	close_pipe_fds(int prev_fd, int pipe_fd[2])
{
	if (prev_fd != -1)
		close(prev_fd);
	if (pipe_fd[0] != -1)
		close(pipe_fd[0]);
	if (pipe_fd[1] != -1)
		close(pipe_fd[1]);
}

static void	child_exec(t_cmd *cmd, t_shell *shell, int prev_fd, int pipe_fd[2])
{
	if (prev_fd != -1 && dup2(prev_fd, STDIN_FILENO) == -1)
		exit(print_error("minishell", "dup2", strerror(errno)));
	if (cmd->next && dup2(pipe_fd[1], STDOUT_FILENO) == -1)
		exit(print_error("minishell", "dup2", strerror(errno)));
	close_pipe_fds(prev_fd, pipe_fd);
	if (!apply_redirs(cmd->redirs))
		exit(1);
	if (is_builtin(cmd))
		exit(exec_builtin(cmd, shell));
	exec_external_child(cmd, shell);
}

static int	close_pipe_and_error(int pipe_fd[2], char *target)
{
	if (pipe_fd[0] != -1)
		close(pipe_fd[0]);
	if (pipe_fd[1] != -1)
		close(pipe_fd[1]);
	return (print_error("minishell", target, strerror(errno)));
}

static void	update_parent_pipe_state(t_cmd *cmd, int *prev_fd,
		int pipe_fd[2], pid_t pid, pid_t *last_pid)
{
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
		return (close_pipe_and_error(pipe_fd, "fork"));
	if (pid == 0)
		child_exec(cmd, shell, *prev_fd, pipe_fd);
	update_parent_pipe_state(cmd, prev_fd, pipe_fd, pid, last_pid);
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