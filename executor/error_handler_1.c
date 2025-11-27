/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler_1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbuquet- <nbuquet-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 20:35:50 by nbuquet-          #+#    #+#             */
/*   Updated: 2025/11/24 20:49:18 by nbuquet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	command_not_found_error(const char *cmd)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": command not found\n", 2);
	return (127);
}

int	no_such_file_error(const char *path)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
	return (127);
}

int	is_a_directory_error(const char *dir)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(dir, 2);
	ft_putstr_fd(": Is a directory\n", 2);
	return (126);
}

int	permission_denied_error(const char *file)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(file, 2);
	ft_putstr_fd(": Permission denied\n", 2);
	return (126);
}
