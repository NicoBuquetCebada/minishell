/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbuquet- <nbuquet-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 16:31:07 by nbuquet-          #+#    #+#             */
/*   Updated: 2025/12/06 15:04:03 by nbuquet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static pid_t	spawn_cmd(t_exec_ctx *ctx, t_command *cmd, int *read_fd);
static void		wait_pids(t_exec_ctx *ctx, pid_t *pids, size_t cmd_c);

int	execute(t_exec_ctx *ctx, t_exec *exec)
{
	size_t	i;
	int		read_fd;
	pid_t	*pids;

	read_fd = -1;
	pids = malloc(exec->cmd_c * sizeof(pid_t));
	if (!pids)
		return (-1);
	i = 0;
	while (i < exec->cmd_c)
	{
		pids[i] = spawn_cmd(ctx, &exec->cmds[i], &read_fd); // Ejecutar builtins. ¿En padre o en hijo?
		if (pids[i] == -1)
			return (wait_pids(ctx, pids, i), free(pids), -1);
		i++;
	}
	wait_pids(ctx, pids, exec->cmd_c);
	free(pids);
	return (0);
}

static pid_t	spawn_cmd(t_exec_ctx *ctx, t_command *cmd, int *read_fd)
{
	pid_t	pid;
	int		pipe_fd[2];

	if (pipe_init(cmd, pipe_fd) == -1)
		return (-1);
	pid = fork();
	if (pid == -1 && cmd->role != TAIL)
		return (close(pipe_fd[0]), close(pipe_fd[1]), -1);
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		connect_childs(cmd, read_fd, pipe_fd);
		close_fds(cmd, read_fd, pipe_fd);
		execute_cmd(ctx, cmd);
		exit(0);
	}
	update_read_fd(cmd, read_fd, pipe_fd);
	return (pid);
}

static void	wait_pids(t_exec_ctx *ctx, pid_t *pids, size_t cmd_c)
{
	size_t	i;
	int		status;

	i = 0;
	while (i < cmd_c)
	{
		if (waitpid(pids[i], &status, 0) != -1) // Implementar retry en EINTR
		{
			if (WIFEXITED(status))
				ctx->last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				ctx->last_status = 128 + WTERMSIG(status);
				// Tratar señales
		}
		i++;
	}
}
