/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbuquet- <nbuquet-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 16:31:07 by nbuquet-          #+#    #+#             */
/*   Updated: 2026/01/26 00:32:40 by nbuquet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"

static int		execute(t_exec_ctx *ctx, t_exec *exec);
static pid_t	spawn_pipe(t_exec_ctx *ctx, t_exec *exec, t_command *cmd,
					int *read_fd);
static void		wait_pids(t_exec_ctx *ctx, pid_t *pids, size_t cmd_c);

int	exec_caller(t_exec_ctx *ctx, t_exec *exec)
{
	if (execute(ctx, exec) == -1)
	{
		perror("minishell");
		g_status = 1;
		return (1);
	}
	return (0);
}

static int	execute(t_exec_ctx *ctx, t_exec *exec)
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
		if (is_builtin_stateful(exec))
			return (free(pids), exec_builtin(ctx, exec, exec->cmds[i].argv, 0));
		pids[i] = spawn_pipe(ctx, exec, &exec->cmds[i], &read_fd);
		if (pids[i] == -1)
			return (wait_pids(ctx, pids, i), free(pids), -1);
		i++;
	}
	wait_pids(ctx, pids, exec->cmd_c);
	free(pids);
	return (0);
}

static pid_t	spawn_pipe(t_exec_ctx *ctx, t_exec *exec, t_command *cmd,
		int *read_fd)
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
		restore_signals();
		connect_childs(cmd, read_fd, pipe_fd);
		close_fds(cmd, read_fd, pipe_fd);
		exec_cmd(ctx, exec, cmd);
	}
	update_read_fd(cmd, read_fd, pipe_fd);
	return (pid);
}

static void	wait_pids(t_exec_ctx *ctx, pid_t *pids, size_t cmd_c)
{
	size_t	i;
	int		status;
	int		ret;

	i = 0;
	while (i < cmd_c)
	{
		while (1)
		{
			ret = waitpid(pids[i], &status, 0);
			if (ret == -1 && errno == EINTR)
				continue ;
			break ;
		}
		if (ret != -1)
		{
			if (WIFEXITED(status))
				g_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				handle_signals(ctx, status);
		}
		i++;
	}
}
