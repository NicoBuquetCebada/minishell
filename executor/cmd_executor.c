/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_executor.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbuquet- <nbuquet-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 20:08:59 by nbuquet-          #+#    #+#             */
/*   Updated: 2025/11/29 20:30:26 by nbuquet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_cmd(t_exec_ctx *ctx, t_command *cmd)
{
	if (process_redirs(cmd) == -1)
		exit(1); // ¿imprimir errores?
	cmd->resolved_path = resolve_path(cmd->argv[0], ctx->envp);
	if (!cmd->resolved_path)
		exit(1);
	execve(cmd->resolved_path, cmd->argv, ctx->envp);
}
