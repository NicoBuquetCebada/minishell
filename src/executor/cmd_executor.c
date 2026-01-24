/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_executor.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbuquet- <nbuquet-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 20:08:59 by nbuquet-          #+#    #+#             */
/*   Updated: 2026/01/24 23:53:03 by nbuquet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"

void	exec_cmd(t_exec_ctx *ctx, t_command *cmd)
{
	if (process_redirs(cmd) == -1)
		ft_error(1);
	if (is_builtin(cmd->argv[0]))
		exec_builtin(ctx, cmd->argv);
	cmd->resolved_path = resolve_path(cmd->argv[0], ctx->envp);
	if (!cmd->resolved_path)
		ft_error(1);
	execve(cmd->resolved_path, cmd->argv, ctx->envp);
	if (errno == ENOEXEC)
		bin_error(cmd->argv[0], ENOEXEC);
	ft_error(126);
}

int	exec_builtin(t_exec_ctx *ctx, char **argv)
{
	size_t	len;
	int		status;

	len = ft_strlen(argv[0]);
	status = 0;
	if (ft_strncmp("echo", argv[0], len) == 0)
		ft_echo(argv);
	if (ft_strncmp("pwd", argv[0], len) == 0)
		ft_pwd();
	if (ft_strncmp("env", argv[0], len) == 0)
		ft_env(ctx->envp);
	if (ft_strncmp("unset", argv[0], len) == 0)
		status = ft_unset(ctx, argv);
	if (ft_strncmp("export", argv[0], len) == 0)
		status = ft_export(ctx, argv);
	if (!ctx->interactive)
		exit(status);
	return (status);
}
