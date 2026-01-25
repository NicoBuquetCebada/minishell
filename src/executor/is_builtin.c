/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbuquet- <nbuquet-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 18:33:04 by nbuquet-          #+#    #+#             */
/*   Updated: 2026/01/25 22:32:47 by nbuquet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"

int	is_builtin(char *cmd)
{
	size_t	len;

	len = ft_strlen(cmd);
	if (ft_strncmp("echo", cmd, len) == 0 || ft_strncmp("pwd", cmd, len) == 0
		|| ft_strncmp("env", cmd, len) == 0 || ft_strncmp("unset", cmd,
			len) == 0 || ft_strncmp("export", cmd, len) == 0 || ft_strncmp("cd",
			cmd, len) == 0 || ft_strncmp("exit", cmd, len) == 0)
		return (1);
	return (0);
}

int	is_builtin_stateful(t_exec *exec)
{
	char	*cmd;
	size_t	len;

	if (exec->cmd_c != 1)
		return (0);
	cmd = exec->cmds[0].argv[0];
	len = ft_strlen(cmd);
	if (ft_strncmp("cd", cmd, len) == 0 || ft_strncmp("export", cmd, len) == 0
		|| ft_strncmp("unset", cmd, len) == 0 || ft_strncmp("exit", cmd,
			len) == 0)
		return (1);
	return (0);
}
