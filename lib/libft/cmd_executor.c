/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_executor.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbuquet- <nbuquet-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 20:08:59 by nbuquet-          #+#    #+#             */
/*   Updated: 2025/12/08 21:42:27 by nbuquet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exec_builtin(char **argv, char **envp);

void	exec_cmd(t_exec_ctx *ctx, t_command *cmd)
{
	if (process_redirs(cmd) == -1)
		ft_error(1);
	if (is_builtin(cmd->argv[0]))
		exec_builtin(cmd->argv, ctx->envp);
	cmd->resolved_path = resolve_path(cmd->argv[0], ctx->envp);
	if (!cmd->resolved_path)
		ft_error(1);
	execve(cmd->resolved_path, cmd->argv, ctx->envp);
	if (errno == ENOEXEC)
		bin_error(cmd->argv[0], ENOEXEC);
	ft_error(126);
}

static void	exec_builtin(char **argv, char **envp)
{
	size_t	len;

	len = ft_strlen(argv[0]);
	if (ft_strncmp("echo", argv[0], len) == 0)
		ft_echo(argv);
	if (ft_strncmp("pwd", argv[0], len) == 0)
		ft_pwd();
	if (ft_strncmp("env", argv[0], len) == 0)
		ft_env(envp);
}
