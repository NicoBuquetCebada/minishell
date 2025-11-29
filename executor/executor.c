/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbuquet- <nbuquet-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 16:31:07 by nbuquet-          #+#    #+#             */
/*   Updated: 2025/11/29 20:33:36 by nbuquet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static pid_t	spawn_cmd(t_exec_ctx *ctx, t_command *cmd, int *read_fd);
static int		pipe_init(t_command *cmd, int pipe_fd[2]);
static void		connect_childs(t_command *cmd, int *read_fd, int pipe_fd[2]);
static void		close_fds(t_command *cmd, int *read_fd, int pipe_fd[2]);
static void		update_read_fd(t_command *cmd, int *read_fd, int pipe_fd[2]);

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
		pids[i] = spawn_cmd(ctx, &exec->cmds[i], &read_fd);
		if (pids[i] == -1)
			return (-1);
		i++;
	}
	// Esperar a los hijos y actualizar $?
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

static int	pipe_init(t_command *cmd, int pipe_fd[2])
{
	if (cmd->role == TAIL)
	{
		pipe_fd[0] = -1;
		pipe_fd[1] = -1;
		return (0);
	}
	if (pipe(pipe_fd) == -1)
		return (-1);
	return (0);
}

static void	connect_childs(t_command *cmd, int *read_fd, int pipe_fd[2])
{
	if (cmd->role != HEAD)
	{
		if (dup2(*read_fd, STDIN_FILENO) == -1)
			exit(1);
	}
	if (cmd->role != TAIL)
	{
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
			exit(1);
	}
}

static void	close_fds(t_command *cmd, int *read_fd, int pipe_fd[2])
{
	if (*read_fd != -1)
		close(*read_fd);
	if (cmd->role != TAIL)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
}

static void	update_read_fd(t_command *cmd, int *read_fd, int pipe_fd[2])
{
	if (*read_fd != -1)
		close(*read_fd);
	if (cmd->role != TAIL)
	{
		close(pipe_fd[1]);
		*read_fd = pipe_fd[0];
	}
	else
		*read_fd = -1;
}
