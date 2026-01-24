/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbuquet- <nbuquet-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 21:17:13 by nbuquet-          #+#    #+#             */
/*   Updated: 2026/01/24 23:56:39 by nbuquet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include <stddef.h>
/*
** Role in the pipeline:
**		HEAD	-> First command of the pipeline
**   MIDLE	-> All the commands in between TAIL and HEAD
**   TAIL	-> Last command in the pipeline,
		or the only one if there is just one
*/
typedef enum e_role
{
	HEAD,
	MIDDLE,
	TAIL
}				t_role;

/*
** Type of redirection:
**   IO_FILE_IN      ->  < file
**   IO_FILE_TRUNC   ->  > file
**   IO_FILE_APPEND  ->  >> file
**   IO_FILE_HEREDOC ->  << delimiter (heredoc)
*/
typedef enum e_iotype
{
	IO_FILE_IN,
	IO_FILE_TRUNC,
	IO_FILE_APPEND,
	IO_FILE_HEREDOC
}				t_iotype;

/*
** A single redirection specification:
** - type   : redirection type
** - arg    : filename or heredoc delimiter
** - expand : for heredocs only (1 = variables expansion, 0 = no expansion)
*/
typedef struct s_iospec
{
	t_iotype	type;
	char		*arg;
	int			expand;
}				t_iospec;

/*
** One command inside a pipeline:
** - argv			: arguments list (argv[0] is the command name)
** - ios			: list of redirections in order of appearance
** - io_c			: number of redirections
** - role			: pipeline role (HEAD/MIDDLE/TAIL)
** - resolved_path	: resolved executable path (filled by executor)
*/
typedef struct s_command
{
	char		**argv;
	t_iospec	*ios;
	size_t		io_c;
	t_role		role;
	char		*resolved_path;
}				t_command;

/*
** A complete pipeline:
** - cmds  : array of commands
** - cmd_c : number of commands in the pipeline
*/
typedef struct s_exec
{
	t_command	*cmds;
	size_t		cmd_c;
}				t_exec;

/*
** Execution context stored globally:
** - envp          : modifiable environment array
** - last_status   : last pipeline exit status (used for $? expansion)
** - interactive   : 1 if shell is interactive, 0 otherwise
*/
typedef struct s_exec_ctx
{
	char		**envp;
	int			last_status;
	int			interactive;
}				t_exec_ctx;

char			*resolve_path(char *cmd, char **envp);
int				is_absolute(char *cmd);
char			*resolve_absolute(char *cmd);
char			*resolve_cmd(char *cmd, char **envp);

int				process_heredocs(t_exec *exec);

int				is_builtin(char *cmd);
int				is_builtin_stateful(t_exec *exec);

int				exec_caller(t_exec_ctx *ctx, t_exec *exec);
int				process_redirs(t_command *cmd);
void			exec_cmd(t_exec_ctx *ctx, t_command *cmd);
int				exec_builtin(t_exec_ctx *ctx, char **argv);
int				pipe_init(t_command *cmd, int pipe_fd[2]);
void			connect_childs(t_command *cmd, int *read_fd, int pipe_fd[2]);
void			close_fds(t_command *cmd, int *read_fd, int pipe_fd[2]);
void			update_read_fd(t_command *cmd, int *read_fd, int pipe_fd[2]);

void			ft_echo(char **argv);
void			ft_pwd(void);
void			ft_env(char **envp);
int				ft_unset(t_exec_ctx *ctx, char **argv);
int				valid_key(char *key);
int				ft_export(t_exec_ctx *ctx, char **argv);
int				find_key(char **envp, char *key);
int				append_entry(t_exec_ctx *ctx, char *entry);
int				replace_entry_at(t_exec_ctx *ctx, char *entry, size_t i);
int				set_kv(t_exec_ctx *ctx, char *arg);
int				ensure_key(t_exec_ctx *ctx, char *key);

int				command_not_found_error(char *cmd);
int				no_such_file_error(char *path, int redir);
int				is_a_directory_error(char *dir, int redir);
int				permission_denied_error(char *file, int redir);
int				exec_format_error(char *cmd);
void			redir_error(char *file, int error);
void			bin_error(char *file, int error);
void			ft_error(int status);
int				ft_cmd_error(char *cmd, char *arg, char *message, int status);

void			handle_signals(t_exec_ctx *ctx, int status);
void			restore_signals(void);

// Parser
char			**dup_envp(char **envp);
void			free_envp(char **envp);

#endif
