/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbuquet- <nbuquet-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 21:19:28 by nbuquet-          #+#    #+#             */
/*   Updated: 2025/11/29 20:32:13 by nbuquet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* echo "hola mundo" | wc -w */
int	main(int ac, char *av[], char *ep[])
{
	t_exec_ctx	ctx;
	t_exec		exec;
	t_command	cmds[2];

	(void)ac;
	(void)av;
	ctx.envp = ep;
	ctx.last_status = 0;
	ctx.interactive = 0;
	cmds[0].argv = (char *[]){"ls", "-l", NULL};
	cmds[0].ios = NULL;
	cmds[0].io_c = 0;
	cmds[0].role = HEAD;
	cmds[0].resolved_path = NULL;
	cmds[1].argv = (char *[]){"wc", "-l", NULL};
	cmds[1].ios = NULL;
	cmds[1].io_c = 0;
	cmds[1].role = TAIL;
	cmds[1].resolved_path = NULL;
	exec.cmds = cmds;
	exec.cmd_c = 2;
	execute(&ctx, &exec);
	return (0);
}
