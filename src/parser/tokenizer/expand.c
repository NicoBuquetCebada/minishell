/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irrevuel <irrevuel@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 22:45:57 by irrevuel          #+#    #+#             */
/*   Updated: 2026/01/23 23:46:38 by irrevuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"
#include "parser.h"

static int	expand_named_var(const char *w, int i, t_exec_ctx *ctx, char **res)
{
	char	*key;
	char	*val;
	int		start;

	start = i;
	while (w[i] && (ft_isalnum((unsigned char)w[i]) || w[i] == '_'))
		i++;
	key = ft_substr(w, start, i - start);
	if (!key)
		return (i);
	val = get_env_value(key, ctx);
	free(key);
	if (val)
	{
		append_text(res, val);
		free(val);
	}
	return (i);
}

static int	expand_var(const char *w, int i, t_exec_ctx *ctx, char **res)
{
	char	*val;

	i++;
	if (!w[i])
		return (append_text(res, "$"), i);
	if (w[i] == '?')
	{
		val = get_env_value("?", ctx);
		if (val)
			append_text(res, val);
		free(val);
		return (i + 1);
	}
	if (!is_var_start(w[i]))
		return (append_text(res, "$"), i);
	return (expand_named_var(w, i, ctx, res));
}

static int	expand_literal(const char *w, int i, char **res)
{
	int		start;
	char	*lit;

	start = i;
	while (w[i] && w[i] != '$')
		i++;
	lit = ft_substr(w, start, i - start);
	if (lit)
	{
		append_text(res, lit);
		free(lit);
	}
	return (i);
}

char	*expand_word(const char *w, t_exec_ctx *ctx)
{
	char	*res;
	int		i;

	res = NULL;
	i = 0;
	while (w[i])
	{
		if (w[i] == '$')
			i = expand_var(w, i, ctx, &res);
		else
			i = expand_literal(w, i, &res);
	}
	if (!res)
		res = ft_strdup("");
	return (res);
}

void	expand(t_token *tokens, t_exec_ctx *ctx)
{
	t_token		*cur;
	t_tokenpart	*part;
	char		*expanded;

	cur = tokens;
	while (cur)
	{
		part = cur->parts;
		while (part)
		{
			if (part->type == WORD && part->origin != IN_SQUOTE)
			{
				expanded = expand_word(part->value, ctx);
				if (expanded)
				{
					free(part->value);
					part->value = expanded;
				}
			}
			part = part->next;
		}
		cur = cur->next;
	}
	merge_words(tokens);
}
