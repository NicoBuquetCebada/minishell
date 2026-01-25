/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   level_controller.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbuquet- <nbuquet-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 23:30:49 by nbuquet-          #+#    #+#             */
/*   Updated: 2026/01/25 23:41:12 by nbuquet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"

void	inc_level(t_exec_ctx *ctx)
{
	char	*val;
	int		lvl;
	char	*lvl_s;
	char	*kv;

	val = get_env(ctx->envp, "SHLVL");
	if (val)
		lvl = ft_atoi(val) + 1;
	else
		lvl = 1;
	lvl_s = ft_itoa(lvl);
	if (!lvl_s)
		return ;
	kv = ft_strjoin("SHLVL=", lvl_s);
	free(lvl_s);
	if (!kv)
		return ;
	set_kv(ctx, kv);
	free(kv);
}
