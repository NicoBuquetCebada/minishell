/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_processer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbuquet- <nbuquet-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 11:25:45 by nbuquet-          #+#    #+#             */
/*   Updated: 2025/11/29 20:28:29 by nbuquet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	redir_stdin(t_iospec io);
static int	redir_stdout(t_iospec io);

int	process_redirs(t_command *cmd)
{
	size_t	i;

	i = 0;
	while (i < cmd->io_c)
	{
		if (cmd->ios[i].type == IO_FILE_IN)
		{
			if (redir_stdin(cmd->ios[i]) == -1)
				return (-1);
		}
		else
		{
			if (redir_stdout(cmd->ios[i]) == -1)
				return (-1);
		}
		i++;
	}
	return (0);
}

static int	redir_stdin(t_iospec io)
{
	int	fd;

	fd = open(io.arg, O_RDONLY);
	if (fd < 0)
		return (-1);
	if (dup2(fd, STDIN_FILENO) == -1)
		return (close(fd), -1);
	return (close(fd));
}

static int	redir_stdout(t_iospec io)
{
	int	fd;

	if (io.type == IO_FILE_APPEND)
		fd = open(io.arg, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else if (io.type == IO_FILE_TRUNC)
		fd = open(io.arg, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else
		fd = -1;
	if (fd < 0)
		return (-1);
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (close(fd), -1);
	return (close(fd));
}
