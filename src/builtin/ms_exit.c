/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssugaya <ssugaya@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-03-23 11:45:50 by ssugaya           #+#    #+#             */
/*   Updated: 2026-03-23 11:45:50 by ssugaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static int	is_numeric_argument(char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	parse_exit_code(char *str, int *status)
{
	int					sign;
	unsigned long long	num;
	unsigned long long	limit;
	int					i;

	if (!is_numeric_argument(str))
		return (0);
	sign = 1;
	num = 0;
	i = 0;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	limit = (unsigned long long)LLONG_MAX;
	if (sign == -1)
		limit++;
	while (str[i])
	{
		if (num > limit / 10
			|| (num == limit / 10
				&& (unsigned long long)(str[i] - '0') > limit % 10))
			return (0);
		num = num * 10 + (str[i] - '0');
		i++;
	}
	*status = (unsigned char)((long long)(sign * (long long)num));
	return (1);
}

static int	print_exit_numeric_error(char *arg)
{
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putendl_fd(": numeric argument required", STDERR_FILENO);
	return (2);
}

int	ms_exit(t_cmd *cmd, t_shell *shell)
{
	int	exit_status;

	if (shell->is_interactive)
		ft_putendl_fd("exit", STDOUT_FILENO);
	if (!cmd || !cmd->argv || !cmd->argv[1])
		exit(shell->last_status);
	if (!parse_exit_code(cmd->argv[1], &exit_status))
		exit(print_exit_numeric_error(cmd->argv[1]));
	if (cmd->argv[2])
		return (print_error("minishell", "exit", "too many arguments"));
	exit(exit_status);
}