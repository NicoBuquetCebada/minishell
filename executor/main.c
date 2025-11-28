/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbuquet- <nbuquet-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 21:19:28 by nbuquet-          #+#    #+#             */
/*   Updated: 2025/11/28 18:00:30 by nbuquet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av)
{
	t_exec		exec;
	t_command	*cmds;
	t_iospec	*ios;
	int			ret;
	int			fd;
	ssize_t		bytes;
	char		buffer[1024];

	if (ac != 2)
	{
		ft_putstr_fd("Usage: ./heredoc_test <delimiter>\n", 2);
		return (1);
	}
	/* allocate one command */
	cmds = (t_command *)malloc(sizeof(t_command));
	if (!cmds)
		return (1);
	cmds[0].argv = NULL;
	cmds[0].resolved_path = NULL;
	cmds[0].role = ROLE_HEAD;

	/* allocate one iospec of type HEREDOC */
	ios = (t_iospec *)malloc(sizeof(t_iospec));
	if (!ios)
	{
		free(cmds);
		return (1);
	}
	ios[0].type = IO_FILE_HEREDOC;
	ios[0].arg = ft_strdup(av[1]);
	ios[0].expand = 0;

	cmds[0].ios = ios;
	cmds[0].io_c = 1;

	exec.cmds = cmds;
	exec.cmd_c = 1;

	ret = heredoc_processer(&exec);
	if (ret != 0)
	{
		ft_putstr_fd("heredoc_processer failed\n", 2);
		free(ios[0].arg);
		free(ios);
		free(cmds);
		return (1);
	}

	ft_putstr_fd("Heredoc converted to file input.\n", 1);
	ft_putstr_fd("New path: ", 1);
	ft_putstr_fd(exec.cmds[0].ios[0].arg, 1);
	ft_putstr_fd("\n", 1);

	/* open and print tmpfile content */
	fd = open(exec.cmds[0].ios[0].arg, O_RDONLY);
	if (fd < 0)
	{
		perror("open");
		free(exec.cmds[0].ios[0].arg);
		free(exec.cmds[0].ios);
		free(exec.cmds);
		return (1);
	}
	ft_putstr_fd("===== HEREDOC FILE CONTENT =====\n", 1);
	while ((bytes = read(fd, buffer, sizeof(buffer))) > 0)
		write(1, buffer, bytes);
	ft_putstr_fd("\n===== END =====\n", 1);
	close(fd);

	/* remove tmpfile */
	unlink(exec.cmds[0].ios[0].arg); 

	free(exec.cmds[0].ios[0].arg);
	free(exec.cmds[0].ios);
	free(exec.cmds);
	return (0);
}
