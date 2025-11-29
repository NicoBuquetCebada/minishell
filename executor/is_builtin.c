/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbuquet- <nbuquet-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 18:33:04 by nbuquet-          #+#    #+#             */
/*   Updated: 2025/11/28 19:51:35 by nbuquet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd)
{
	size_t len;

	len = ft_strlen(cmd);
	if (ft_strcmp("echo", cmd, len) == 0 || ft_strcmp("cd", cmd, len) == 0
		|| ft_strcmp("pwd", cmd, len) == 0 || ft_strcmp("export", cmd, len) == 0
		|| ft_strcmp("unset", cmd, len) == 0 || ft_strcmp("env", cmd, len) == 0
		|| ft_strcmp("exit", cmd, len) == 0)
		return (1);
	return (0);
}

int	is_builtin_statefull(t_exec *exec)
{
	char	*cmd;
	size_t	len;

	if (exec->cmd_c != 1)
		return (0);
	cmd = exec->cmds[0].argv[0];
	len = ft_strlen(cmd);
	if (ft_strcmp("cd", cmd, len) == 0 || ft_strcmp("export", cmd, len) == 0
		|| ft_strcmp("unset", cmd, len) == 0 || ft_strcmp("exit", cmd,
			len) == 0)
		return (1);
	return (0);
}
