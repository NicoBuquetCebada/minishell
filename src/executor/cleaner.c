/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbuquet- <nbuquet-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 21:39:17 by nbuquet-          #+#    #+#             */
/*   Updated: 2026/01/25 22:47:11 by nbuquet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"

static void	clean_exec(t_exec *exec);
static void	clean_cmd(t_command *cmd);
static void	free_array(char **v);

void	clean_shell(t_exec_ctx *ctx, t_exec *exec)
{
	clean_exec(exec);
	clean_ctx(ctx);
}

static void	clean_exec(t_exec *exec)
{
	size_t	i;

	if (!exec || !exec->cmds)
		return ;
	i = 0;
	while (i < exec->cmd_c)
		clean_cmd(&exec->cmds[i++]);
	free(exec->cmds);
	exec->cmds = NULL;
	exec->cmd_c = 0;
}

void	clean_ctx(t_exec_ctx *ctx)
{
	if (!ctx)
		return ;
	free_array(ctx->envp);
	ctx->envp = NULL;
	free(ctx->wd);
	free(ctx->old_wd);
	ctx->wd = NULL;
	ctx->old_wd = NULL;
}

static void	clean_cmd(t_command *cmd)
{
	size_t	i;

	free_array(cmd->argv);
	if (cmd->ios)
	{
		i = 0;
		while (i < cmd->io_c)
			free(cmd->ios[i++].arg);
		free(cmd->ios);
	}
	free(cmd->resolved_path);
	cmd->argv = NULL;
	cmd->ios = NULL;
	cmd->io_c = 0;
	cmd->resolved_path = NULL;
}

static void	free_array(char **v)
{
	size_t	i;

	if (!v)
		return ;
	i = 0;
	while (v[i])
		free(v[i++]);
	free(v);
}
