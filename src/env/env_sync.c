/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_sync.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssugaya <ssugaya@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-03-25 06:56:54 by ssugaya           #+#    #+#             */
/*   Updated: 2026-03-25 06:56:54 by ssugaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int	shell_refresh_envp(t_shell *shell)
{
	char	**new_envp;

	if (!shell)
		return (0);
	new_envp = env_to_envp(shell->env_list);
	if (!new_envp)
		return (0);
	free_envp(shell->envp);
	shell->envp = new_envp;
	return (1);
}