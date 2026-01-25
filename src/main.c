/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbuquet- <nbuquet-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 21:19:28 by nbuquet-          #+#    #+#             */
/*   Updated: 2026/01/26 00:34:04 by nbuquet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"
#include "parser.h"

volatile sig_atomic_t	g_status = 0;

int	main(int argc, char *argv[], char *envp[])
{
	char		*line;
	t_exec_ctx	ctx;

	(void)argc;
	(void)argv;
	ctx.envp = dup_envp(envp);
	ctx.interactive = isatty(STDIN_FILENO);
	ctx.old_wd = NULL;
	ctx.wd = dup_cwd();
	inc_level(&ctx);
	while (1)
	{
		line = readline("[minishell]$ ");
		add_history(line);
		handle_input(line, &ctx);
		free(line);
	}
	// clear_history();
	clean_ctx(&ctx);
	return (0);
}
