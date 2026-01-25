/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbuquet- <nbuquet-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 22:58:26 by nbuquet-          #+#    #+#             */
/*   Updated: 2026/01/25 12:05:30 by nbuquet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"

int	ft_export(t_exec_ctx *ctx, char **argv)
{
	size_t	i;
	int		status;

	status = 0;
	i = 1;
	if (!argv[i])
		return (ft_env(ctx->envp), 0);
	while (argv[i])
	{
		if (ft_strchr(argv[i], '=') != NULL)
		{
			if (set_kv(ctx, argv[i]) != 0)
				status = ft_error_msg("export", argv[i],
						"not a valid identifier", 1);
		}
		else
		{
			if (!valid_key(argv[i]) || ensure_key(ctx, argv[i]) != 0)
				status = ft_error_msg("export", argv[i],
						"not a valid identifier", 1);
		}
		i++;
	}
	return (status);
}
