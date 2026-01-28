/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbuquet- <nbuquet-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 21:45:54 by nbuquet-          #+#    #+#             */
/*   Updated: 2026/01/28 17:27:25 by nbuquet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"

static int			get_status(char *arg, int *error);
static long long	atoi_long_long(const char *str, int *error);
static int			out_of_range(int neg, unsigned long long num, int *error);

int	ft_exit(t_exec_ctx *ctx, t_exec *exec, int child)
{
	int	error;

	error = 0;
	if (ctx && ctx->interactive && exec && exec->cmd_c == 1 && !child)
		ft_putendl_fd("exit", 2);
	if (exec && exec->cmds && exec->cmds->argv && exec->cmds->argv[1])
	{
		g_status = get_status(exec->cmds->argv[1], &error);
		if (error)
			g_status = ft_error_msg("exit", exec->cmds->argv[1],
					"numeric argument required", 2);
		else if (exec->cmds->argv[2])
		{
			g_status = ft_error_msg("exit", NULL, "too many arguments", 1);
			if (!child)
				return (g_status);
		}
	}
	if (!child)
		clean_shell(ctx, exec);
	return (exit(g_status), 2);
}

static int	get_status(char *arg, int *error)
{
	unsigned long long	i;

	if (!arg)
		return (g_status);
	i = 0;
	while (ft_isspace(arg[i]))
		i++;
	if (arg[i] == '\0')
		*error = 1;
	if (arg[i] == '-' || arg[i] == '+')
		i++;
	if (!ft_isdigit(arg[i]))
		*error = 1;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]) && !ft_isspace(arg[i]))
			*error = 1;
		i++;
	}
	i = atoi_long_long(arg, error);
	return (i % 256);
}

static long long	atoi_long_long(const char *str, int *error)
{
	unsigned long long	num;
	int					neg;
	int					i;

	num = 0;
	neg = 1;
	i = 0;
	while (str[i] && ft_isspace(str[i]))
		i++;
	if (str[i] == '+')
		i++;
	else if (str[i] == '-')
	{
		neg *= -1;
		i++;
	}
	while (str[i] && ft_isdigit(str[i]))
	{
		num = (num * 10) + (str[i] - '0');
		if (out_of_range(neg, num, error))
			break ;
		i++;
	}
	return (num * neg);
}

static int	out_of_range(int neg, unsigned long long num, int *error)
{
	if ((neg == 1 && num > LONG_MAX) || (neg == -1 && num >
			-(unsigned long)LONG_MIN))
		*error = 1;
	return (*error);
}
