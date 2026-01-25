/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irrevuel <irrevuel@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 23:51:36 by irrevuel          #+#    #+#             */
/*   Updated: 2026/01/23 23:51:54 by irrevuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"
#include "parser.h"

char	*get_env_value(const char *key, t_exec_ctx *ctx)
{
	int		i;
	char	*env;
	char	*sep;
	size_t	key_len;

	if (ft_strncmp(key, "?", 2) == 0)
		return (ft_itoa(ctx->last_status));
	key_len = ft_strlen(key);
	i = 0;
	while (ctx->envp[i])
	{
		env = ctx->envp[i];
		sep = ft_strchr(env, '=');
		if (sep && (size_t)(sep - env) == key_len && ft_strncmp(env, key,
				key_len) == 0)
			return (ft_strdup(sep + 1));
		i++;
	}
	return (NULL);
}

void	append_text(char **result, const char *text)
{
	char	*temp;

	if (!text)
		return ;
	if (*result)
	{
		temp = ft_strjoin(*result, text);
		free(*result);
		*result = temp;
	}
	else
		*result = ft_strdup(text);
}

int	is_var_start(char c)
{
	return (ft_isalpha((unsigned char)c) || c == '_');
}
