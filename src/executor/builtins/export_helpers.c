/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbuquet- <nbuquet-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 23:48:50 by nbuquet-          #+#    #+#             */
/*   Updated: 2026/01/24 23:57:37 by nbuquet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"

int	set_kv(t_exec_ctx *ctx, char *arg)
{
	char	*eq;
	char	*key;
	char	*entry;
	int		i;

	eq = ft_strchr(arg, '=');
	key = ft_substr(arg, 0, eq - arg);
	if (!valid_key(key))
		return (free(key), -1);
	entry = ft_strdup(arg);
	if (!entry)
		return (free(key), -1);
	i = find_key(ctx->envp, key);
	free(key);
	if (i >= 0)
		return (replace_entry_at(ctx, entry, i));
	return (append_entry(ctx, entry));
}

int	ensure_key(t_exec_ctx *ctx, char *key)
{
	char	*entry;
	int		i;

	if (!valid_key(key))
		return (-1);
	i = find_key(ctx->envp, key);
	if (i >= 0)
		return (0);
	entry = ft_strjoin(key, "=");
	if (!entry)
		return (-1);
	return (append_entry(ctx, entry));
}

int	find_key(char **envp, char *key)
{
	size_t	i;
	size_t	len;

	if (!envp || !key)
		return (-1);
	len = ft_strlen(key);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], key, len) == 0 && envp[i][len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

int	replace_entry_at(t_exec_ctx *ctx, char *entry, size_t i)
{
	if (!ctx || !ctx->envp || !entry)
		return (-1);
	free(ctx->envp[i]);
	ctx->envp[i] = entry;
	return (0);
}

int	append_entry(t_exec_ctx *ctx, char *entry)
{
	size_t	i;
	char	**new_env;

	if (!ctx || !ctx->envp || !entry)
		return (-1);
	i = 0;
	while (ctx->envp[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 2));
	if (!new_env)
		return (-1);
	i = 0;
	while (ctx->envp[i])
	{
		new_env[i] = ctx->envp[i];
		i++;
	}
	new_env[i++] = entry;
	new_env[i] = NULL;
	free(ctx->envp);
	ctx->envp = new_env;
	return (0);
}
