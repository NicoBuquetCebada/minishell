/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler_1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbuquet- <nbuquet-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 20:35:50 by nbuquet-          #+#    #+#             */
/*   Updated: 2026/01/19 19:40:43 by nbuquet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "executor.h"

int	command_not_found_error(char *cmd)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": command not found\n", 2);
	return (127);
}

int	no_such_file_error(char *path, int redir)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
	if (redir)
		return (1);
	return (127);
}

int	is_a_directory_error(char *dir, int redir)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(dir, 2);
	ft_putstr_fd(": Is a directory\n", 2);
	if (redir)
		return (1);
	return (126);
}

int	permission_denied_error(char *file, int redir)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(file, 2);
	ft_putstr_fd(": Permission denied\n", 2);
	if (redir)
		return (1);
	return (126);
}

int	exec_format_error(char *cmd)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": Exec format error\n", 2);
	return (126);
}

void	redir_error(char *file, int error)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(file, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(error), 2);
	ft_putchar_fd('\n', 2);
	exit(1);
}

void	bin_error(char *file, int error)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(file, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(error), 2);
	ft_putchar_fd('\n', 2);
	if (error == ENOENT)
		exit(127);
	if (error == EISDIR || error == EACCES || error == ENOEXEC)
		exit(126);
	exit(1);
}

void	ft_error(int status)
{
	perror("minishell");
	exit(status);
}
