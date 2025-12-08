/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbuquet- <nbuquet-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 14:27:58 by nbuquet-          #+#    #+#             */
/*   Updated: 2025/12/08 14:31:40 by nbuquet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_signals(t_exec_ctx *ctx, int status)
{
	int	sigv;

	sigv = WTERMSIG(status);
	ctx->last_status = 128 + sigv;
	if (sigv == 2)
		ft_putchar_fd('\n', 1);
	if (sigv == 3)
		ft_putstr_fd("Quit (core dumped)\n", 1);
}

void	restore_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
