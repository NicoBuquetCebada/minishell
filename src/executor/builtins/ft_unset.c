/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbuquet- <nbuquet-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 22:48:33 by nbuquet-          #+#    #+#             */
/*   Updated: 2026/01/26 00:23:22 by nbuquet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"

static int	remove_env(t_exec_ctx *ctx, char *key);
static int	env_cmp(char *e1, char *e2);

int	ft_unset(t_exec_ctx *ctx, char **argv)
{
	int	i;
	int	status;

	status = 0;
	i = 1;
	while (argv[i])
	{
		if (!valid_key(argv[i]) || ft_strchr(argv[i], '=') != NULL)
			status = ft_error_msg("unset", argv[i], "not a valid identifier",
					1);
		else
			remove_env(ctx, argv[i]);
		i++;
	}
	return (status);
}

int	valid_key(char *key)
{
	size_t	i;

	i = 0;
	if (!key || !key[i])
		return (0);
	if (ft_isalpha(key[i]) == 0 && key[i] != '_')
		return (0);
	i++;
	while (key[i] && key[i] != '=')
	{
		if (ft_isalnum(key[i]) == 0 && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static int	remove_env(t_exec_ctx *ctx, char *key)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (!ctx || !ctx->envp || !key || !key[i])
		return (-1);
	while (ctx->envp[i])
	{
		if (env_cmp(ctx->envp[i], key))
		{
			free(ctx->envp[i]);
			j = i;
			while (ctx->envp[j])
			{
				ctx->envp[j] = ctx->envp[j + 1];
				j++;
			}
			return (0);
		}
		i++;
	}
	return (-1);
}

static int	env_cmp(char *e1, char *e2)
{
	size_t	len;

	if (!e1 || !e2)
		return (0);
	len = ft_strlen(e2);
	if (ft_strncmp(e1, e2, len) != 0)
		return (0);
	if (e1[len] != '=')
		return (0);
	return (1);
}
