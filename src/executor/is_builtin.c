/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irrevuel <irrevuel@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 18:33:04 by nbuquet-          #+#    #+#             */
/*   Updated: 2026/01/27 23:13:20 by irrevuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"

int	is_builtin(char *cmd)
{
	if (ft_strcmp("echo", cmd) == 0 || ft_strcmp("pwd", cmd) == 0
		|| ft_strcmp("env", cmd) == 0 || ft_strcmp("unset", cmd) == 0 || ft_strcmp("export", cmd) == 0 || ft_strcmp("cd",
			cmd) == 0 || ft_strcmp("exit", cmd) == 0)
		return (1);
	return (0);
}

int	is_builtin_stateful(t_exec *exec)
{
	char	*cmd;

	if (exec->cmd_c != 1)
		return (0);
	cmd = exec->cmds[0].argv[0];
	if (ft_strcmp("cd", cmd) == 0 || ft_strcmp("export", cmd) == 0
		|| ft_strcmp("unset", cmd) == 0 || ft_strcmp("exit", cmd) == 0)
		return (1);
	return (0);
}
