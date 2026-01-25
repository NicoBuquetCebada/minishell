/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler_1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbuquet- <nbuquet-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 20:35:50 by nbuquet-          #+#    #+#             */
/*   Updated: 2026/01/25 19:43:40 by nbuquet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "executor.h"

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

int	ft_error_msg(char *cmd, char *arg, char *message, int status)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
	if (arg)
	{
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putstr_fd(message, 2);
	ft_putchar_fd('\n', 2);
	return (status);
}

int	cd_error(char *path)
{
	if (errno == ESTALE)
		errno = ENOENT;
	ft_error_msg("cd", path, strerror(errno), errno);
	return (0);
}
