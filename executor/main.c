/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbuquet- <nbuquet-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 21:19:28 by nbuquet-          #+#    #+#             */
/*   Updated: 2025/12/06 15:07:29 by nbuquet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// TODO: Manejo de errores y respuestas en señales, builtins, ¿interactive?

// Main Tester
static void	test_pipeline_redirs(t_exec_ctx *ctx)
{
	t_exec		exec;
	t_command	cmds[3];
	t_iospec	ios0[1];
	t_iospec	ios2[1];

	ios0[0].type = IO_FILE_IN;
	ios0[0].arg = "input.txt";
	ios0[0].expand = 0;
	cmds[0].argv = (char *[]){"cat", NULL};
	cmds[0].ios = ios0;
	cmds[0].io_c = 1;
	cmds[0].role = HEAD;
	cmds[0].resolved_path = NULL;
	cmds[1].argv = (char *[]){"grep", "foo", NULL};
	cmds[1].ios = NULL;
	cmds[1].io_c = 0;
	cmds[1].role = MIDDLE;
	cmds[1].resolved_path = NULL;
	ios2[0].type = IO_FILE_APPEND;
	ios2[0].arg = "output.txt";
	ios2[0].expand = 0;
	cmds[2].argv = (char *[]){"wc", "-l", NULL};
	cmds[2].ios = ios2;
	cmds[2].io_c = 1;
	cmds[2].role = TAIL;
	cmds[2].resolved_path = NULL;
	exec.cmds = cmds;
	exec.cmd_c = 3;
	execute(ctx, &exec);
}

static void	test_missing_infile(t_exec_ctx *ctx)
{
	t_exec		exec;
	t_command	cmd;
	t_iospec	ios[1];

	ios[0].type = IO_FILE_IN;
	ios[0].arg = "no_such_file.txt";
	ios[0].expand = 0;
	cmd.argv = (char *[]){"cat", NULL};
	cmd.ios = ios;
	cmd.io_c = 1;
	cmd.role = TAIL;
	cmd.resolved_path = NULL;
	exec.cmds = &cmd;
	exec.cmd_c = 1;
	execute(ctx, &exec);
}

static void	test_cmd_not_found(t_exec_ctx *ctx)
{
	t_exec		exec;
	t_command	cmd;

	cmd.argv = (char *[]){"no_such_command_42", "arg1", NULL};
	cmd.ios = NULL;
	cmd.io_c = 0;
	cmd.role = TAIL;
	cmd.resolved_path = NULL;
	exec.cmds = &cmd;
	exec.cmd_c = 1;
	execute(ctx, &exec);
}

static void	test_pipe_broken_by_redir(t_exec_ctx *ctx)
{
	t_exec		exec;
	t_command	cmds[3];
	t_iospec	ios1[1];

	cmds[0].argv = (char *[]){"echo", "hola", NULL};
	cmds[0].ios = NULL;
	cmds[0].io_c = 0;
	cmds[0].role = HEAD;
	cmds[0].resolved_path = NULL;
	ios1[0].type = IO_FILE_TRUNC;
	ios1[0].arg = "mid.txt";
	ios1[0].expand = 0;
	cmds[1].argv = (char *[]){"cat", NULL};
	cmds[1].ios = ios1;
	cmds[1].io_c = 1;
	cmds[1].role = MIDDLE;
	cmds[1].resolved_path = NULL;
	cmds[2].argv = (char *[]){"wc", "-l", NULL};
	cmds[2].ios = NULL;
	cmds[2].io_c = 0;
	cmds[2].role = TAIL;
	cmds[2].resolved_path = NULL;
	exec.cmds = cmds;
	exec.cmd_c = 3;
	execute(ctx, &exec);
}

static void	test_multi_redir_out(t_exec_ctx *ctx)
{
	t_exec		exec;
	t_command	cmd;
	t_iospec	ios[2];

	ios[0].type = IO_FILE_TRUNC;
	ios[0].arg = "a.txt";
	ios[0].expand = 0;
	ios[1].type = IO_FILE_TRUNC;
	ios[1].arg = "b.txt";
	ios[1].expand = 0;
	cmd.argv = (char *[]){"echo", "multi", NULL};
	cmd.ios = ios;
	cmd.io_c = 2;
	cmd.role = TAIL;
	cmd.resolved_path = NULL;
	exec.cmds = &cmd;
	exec.cmd_c = 1;
	execute(ctx, &exec);
}

int	main(int ac, char *av[], char *ep[])
{
	t_exec_ctx	ctx;

	(void)ac;
	(void)av;
	ctx.envp = ep;
	ctx.last_status = 0;
	ctx.interactive = 0;
	test_pipeline_redirs(&ctx);
	test_missing_infile(&ctx);
	test_cmd_not_found(&ctx);
	test_pipe_broken_by_redir(&ctx);
	test_multi_redir_out(&ctx);
	return (0);
}
