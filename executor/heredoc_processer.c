/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_processer.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbuquet- <nbuquet-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 21:45:15 by nbuquet-          #+#    #+#             */
/*   Updated: 2025/11/29 12:05:48 by nbuquet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	g_heredoc_id = 0;
static int	convert_to_filein(t_iospec *io);
static char	*till_delimiter(t_iospec io);
static char	*new_tmpfile(char *content);
static char	*tmpfile_path(void);

int	process_heredocs(t_exec *exec)
{
	size_t		i;
	size_t		j;
	t_iospec	*io;

	i = 0;
	while (i < exec->cmd_c)
	{
		j = 0;
		while (j < exec->cmds[i].io_c)
		{
			io = &exec->cmds[i].ios[j];
			if (io->type == IO_FILE_HEREDOC)
				if (convert_to_filein(io) != 0)
					return (-1);
			j++;
		}
		i++;
	}
	return (0);
}

static int	convert_to_filein(t_iospec *io)
{
	char	*content;
	char	*path;

	content = till_delimiter(*io);
	if (!content)
		return (-1);
	path = new_tmpfile(content);
	free(content);
	if (!path)
		return (-1);
	free(io->arg);
	io->arg = path;
	io->type = IO_FILE_IN;
	return (0);
}

static char	*till_delimiter(t_iospec io)
{
	int		del_len;
	char	*line;
	char	*dst;
	char	*tmp;

	dst = NULL;
	del_len = ft_strlen(io.arg);
	while (1)
	{
		line = readline("> ");
		if (!line)
			return (dst);
		if (ft_strncmp(line, io.arg, del_len) == 0)
			return (free(line), dst);
		tmp = dst;
		if (!dst)
			dst = ft_strdup(line);
		else
			dst = ft_strjoin(dst, line);
		free(tmp);
		free(line);
	}
}

static char	*new_tmpfile(char *content)
{
	int		fd;
	char	*path;

	path = tmpfile_path();
	if (!path)
		return (NULL);
	fd = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd < 0)
		return (free(path), NULL);
	write(fd, content, ft_strlen(content));
	close(fd);
	return (path);
}

static char	*tmpfile_path(void)
{
	char	*id;
	char	*name;

	id = ft_itoa(g_heredoc_id);
	if (!id)
		return (NULL);
	name = ft_strjoin("/tmp/ms_heredoc_", id);
	free(id);
	if (!name)
		return (NULL);
	g_heredoc_id++;
	return (name);
}
