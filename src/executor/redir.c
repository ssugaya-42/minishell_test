/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssugaya <ssugaya@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-03-21 15:43:47 by ssugaya           #+#    #+#             */
/*   Updated: 2026-03-21 15:43:47 by ssugaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include <fcntl.h>
#include <unistd.h>

static int	open_redir_file(t_redir *redir)
{
	int	fd;

	fd = -1;
	if (redir->type == TOKEN_REDIR_IN)
		fd = open(redir->file, O_RDONLY);
	else if (redir->type == TOKEN_REDIR_OUT)
		fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == TOKEN_APPEND)
		fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	return (fd);
}

static int	redirect_fd(t_redir *redir, int fd)
{
	if (redir->type == TOKEN_REDIR_IN)
	{
		if (dup2(fd, STDIN_FILENO) == -1)
		{
			close(fd);
			return (0);
		}
	}
	else if (redir->type == TOKEN_REDIR_OUT || redir->type == TOKEN_APPEND)
	{
		if (dup2(fd, STDOUT_FILENO) == -1)
		{
			close(fd);
			return (0);
		}
	}
	close(fd);
	return (1);
}

int	apply_redirs(t_redir *redirs)
{
	int	fd;

	while (redirs)
	{
		if (redirs->type == TOKEN_HEREDOC)
		{
			redirs = redirs->next;
			continue ;
		}
		fd = open_redir_file(redirs);
		if (fd < 0)
		{
			print_error("minishell", redirs->file,
				"failed to open redirection file");
			return (0);
		}
		if (!redirect_fd(redirs, fd))
		{
			print_error("minishell", redirs->file,
				"failed to redirect file descriptor");
			return (0);
		}
		redirs = redirs->next;
	}
	return (1);
}

int	backup_stdio(t_shell *shell)
{
	shell->stdin_backup = dup(STDIN_FILENO);
	if (shell->stdin_backup == -1)
		return (print_error("minishell", "dup", "failed to backup stdin"));
	shell->stdout_backup = dup(STDOUT_FILENO);
	if (shell->stdout_backup == -1)
	{
		close(shell->stdin_backup);
		shell->stdin_backup = -1;
		return (print_error("minishell", "dup", "failed to backup stdout"));
	}
	return (1);
}

int	restore_stdio(t_shell *shell)
{
	if (shell->stdin_backup != -1)
	{
		if (dup2(shell->stdin_backup, STDIN_FILENO) == -1)
			return (print_error("minishell", "dup2", "failed to restore stdin"));
		close(shell->stdin_backup);
		shell->stdin_backup = -1;
	}
	if (shell->stdout_backup != -1)
	{
		if (dup2(shell->stdout_backup, STDOUT_FILENO) == -1)
			return (print_error("minishell", "dup2", "failed to restore stdout"));
		close(shell->stdout_backup);
		shell->stdout_backup = -1;
	}
	return (1);
}