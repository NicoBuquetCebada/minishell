/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbuquet- <nbuquet-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 13:37:04 by nbuquet-          #+#    #+#             */
/*   Updated: 2025/12/08 21:44:13 by nbuquet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	no_newline(char *arg);

void	ft_echo(char **argv)
{
	size_t	i;
	int		no_nl;

	if (!argv[1])
		return (ft_putchar_fd('\n', 1), exit(0));
	i = 1;
	no_nl = 0;
	if (no_newline(argv[i]))
	{
		no_nl = 1;
		i++;
	}
	while (argv[i] && no_newline(argv[i]))
		i++;
	while (argv[i])
	{
		ft_putstr_fd(argv[i], 1);
		if (argv[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (!no_nl)
		ft_putchar_fd('\n', 1);
	exit(0);
}

static int	no_newline(char *arg)
{
	size_t	i;

	i = 0;
	if (arg[i] != '-')
		return (0);
	i++;
	while (arg[i] && arg[i] == 'n')
		i++;
	if (arg[i])
		return (0);
	return (1);
}
