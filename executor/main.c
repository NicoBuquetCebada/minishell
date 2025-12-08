/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbuquet- <nbuquet-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 21:19:28 by nbuquet-          #+#    #+#             */
/*   Updated: 2025/12/08 23:52:55 by nbuquet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// TODO: Built-ins: cd | export | unset | exit, interactive (ver si me incumbe de algún modo)

// Main child built-ins tester
static void	run_simple_cmd(t_exec_ctx *ctx, char **argv);

int	main(int argc, char **argv, char **envp)
{
	t_exec_ctx	ctx;
	char		*echo_args1[] = {"echo", "hello", "world", NULL};
	char		*echo_args2[] = {"echo", "-n", "-nnn", "no newline", NULL};
	char		*pwd_args[] = {"pwd", NULL};
	char		*env_args[] = {"env", NULL};
	
	(void)argc;
	(void)argv;
	ctx.envp = envp;
	ctx.last_status = 0;
	ctx.interactive = 0;
	printf("=== echo hello world ===\n");
	run_simple_cmd(&ctx, echo_args1);
	printf("=== echo -n -nnn no newline ===\n");
	run_simple_cmd(&ctx, echo_args2);
	printf("=== pwd ===\n");
	run_simple_cmd(&ctx, pwd_args);
	printf("=== env ===\n");
	run_simple_cmd(&ctx, env_args);
	return (0);
}

static void	run_simple_cmd(t_exec_ctx *ctx, char **argv)
{
	t_command	cmd;
	t_exec		exec;

	cmd.argv = argv;
	cmd.ios = NULL;
	cmd.io_c = 0;
	cmd.role = TAIL;
	cmd.resolved_path = NULL;
	exec.cmds = &cmd;
	exec.cmd_c = 1;
	exec_caller(ctx, &exec);
}
