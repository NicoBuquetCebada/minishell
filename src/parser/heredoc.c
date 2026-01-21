#include "minishell.h"
#include "executor.h"
#include "parser.h"

static int	is_delim(char *line, char *delim)
{
	return (line && delim && ft_strcmp(line, delim) == 0);
}

static int	write_ln(int fd, char *s)
{
	if (!s)
		return (1);
	if (write(fd, s, ft_strlen(s)) < 0)
		return (0);
	if (write(fd, "\n", 1) < 0)
		return (0);
	return (1);
}

static char	*mk_tmp_name(int n)
{
	char	*i;
	char	*tmp;
	char	*name;

	i = ft_itoa(n);
	if (!i)
		return (NULL);
	tmp = ft_strjoin("/tmp/.ms_hd_", i);
	free(i);
	if (!tmp)
		return (NULL);
	name = tmp;
	return (name);
}

static int	open_unique_tmp(char **out_path)
{
	int		fd;
	int		n;
	char	*name;

	n = 0;
	while (n < 10000)
	{
		name = mk_tmp_name(n++);
		if (!name)
			return (-1);
		fd = open(name, O_CREAT | O_EXCL | O_WRONLY, 0600);
		if (fd >= 0)
			return (*out_path = name, fd);
		free(name);
	}
	return (-1);
}

static int	fill_tmpfile(int fd, t_iospec *io, t_exec_ctx *ctx)
{
	char	*line;
	char	*out;

	while (1)
	{
		line = readline("> ");
		if (!line || is_delim(line, io->arg))
			break ;
		out = line;
		if (io->expand == 1)
			out = expand_word(line, ctx);
		if (!write_ln(fd, out))
        {
            if (io->expand == 1)
                free(out);
            free(line);
            return (0);
        }
		if (io->expand == 1)
			free(out);
		free(line);
	}
	free(line);
	return (1);
}

static int	one_heredoc(t_iospec *io, t_exec_ctx *ctx)
{
	int		fd;
	char	*path;

	fd = open_unique_tmp(&path);
	if (fd < 0)
		return (0);
	if (!fill_tmpfile(fd, io, ctx))
		return (close(fd), unlink(path), free(path), 0);
	close(fd);
	free(io->arg);
	io->arg = path;
	io->type = IO_FILE_IN;
	io->expand = -1;
	return (1);
}

int	process_heredocs(t_exec *exec, t_exec_ctx *ctx)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (exec && i < exec->cmd_c)
	{
		j = 0;
		while (j < exec->cmds[i].io_c)
		{
			if (exec->cmds[i].ios[j].type == IO_FILE_HEREDOC)
				if (!one_heredoc(&exec->cmds[i].ios[j], ctx))
					return (0);
			j++;
		}
		i++;
	}
	return (1);
}
