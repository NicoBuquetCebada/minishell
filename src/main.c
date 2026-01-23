/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irrevuel <irrevuel@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 21:19:28 by nbuquet-          #+#    #+#             */
/*   Updated: 2026/01/24 00:33:13 by irrevuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"
#include "parser.h"

int	main(int argc, char *argv[], char *envp[])
{
	char		*line;
	t_exec_ctx	ctx;

	(void)argc;
	(void)argv;
	(void)envp;
	ctx.envp = envp;
	ctx.last_status = 0;
	ctx.interactive = 1;
	while (1)
	{
		line = readline("minishell-1.0 $ ");
		add_history(line);
		handle_input(line, &ctx);
		free(line);
	}
	// clear_history()
	return (0);
}
