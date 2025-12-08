/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_executor.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbuquet- <nbuquet-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 20:08:59 by nbuquet-          #+#    #+#             */
/*   Updated: 2025/12/08 13:17:34 by nbuquet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_cmd(t_exec_ctx *ctx, t_command *cmd)
{
	if (process_redirs(cmd) == -1)
		ft_error(1);
	cmd->resolved_path = resolve_path(cmd->argv[0], ctx->envp);
	if (!cmd->resolved_path)
		ft_error(1);
	execve(cmd->resolved_path, cmd->argv, ctx->envp);
	if (errno == ENOEXEC)
		bin_error(cmd->argv[0], ENOEXEC);
	ft_error(126);
}
