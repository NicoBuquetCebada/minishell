/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irrevuel <irrevuel@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 21:19:28 by nbuquet-          #+#    #+#             */
/*   Updated: 2026/01/28 19:21:31 by irrevuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"
#include "parser.h"

volatile sig_atomic_t	g_status = 0;

static void	init_ctx(t_exec_ctx *ctx, char **envp)
{
	ctx->envp = dup_envp(envp);
	ctx->interactive = isatty(STDIN_FILENO);
	ctx->old_wd = NULL;
	ctx->wd = dup_cwd();
	inc_level(ctx);
}

static void	run_interactive(t_exec_ctx *ctx)
{
	char	*line;

	line = NULL;
	while (1)
	{
		signal_handlers();
		line = readline("[minishell]$ ");
		if (line && line[0] != '\0')
		{
			add_history(line);
			handle_input(line, ctx);
		}
		handle_eof(ctx, line);
		free(line);
	}
	clear_history();
}

static void	run_noninteractive(t_exec_ctx *ctx)
{
	char	*line;

	line = NULL;
	while (1)
	{
		line = readline("");
		if (!line)
			break ;
		if (line[0] != '\0')
			handle_input(line, ctx);
		free(line);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_exec_ctx	ctx;

	if (argc > 1 && argv)
	{
		ft_putstr_fd("minishell doesn't get any arguments", 2);
		return (0);
	}
	init_ctx(&ctx, envp);
	signal_handlers();
	if (ctx.interactive)
		run_interactive(&ctx);
	else
		run_noninteractive(&ctx);
	clean_ctx(&ctx);
}
