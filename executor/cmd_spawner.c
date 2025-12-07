/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_spawner.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbuquet- <nbuquet-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 14:45:27 by nbuquet-          #+#    #+#             */
/*   Updated: 2025/12/06 15:11:52 by nbuquet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pipe_init(t_command *cmd, int pipe_fd[2])
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

void	connect_childs(t_command *cmd, int *read_fd, int pipe_fd[2])
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

void	close_fds(t_command *cmd, int *read_fd, int pipe_fd[2])
{
	if (*read_fd != -1)
		close(*read_fd);
	if (cmd->role != TAIL)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
}

void	update_read_fd(t_command *cmd, int *read_fd, int pipe_fd[2])
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
