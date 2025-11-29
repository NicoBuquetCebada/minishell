/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbuquet- <nbuquet-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 21:17:13 by nbuquet-          #+#    #+#             */
/*   Updated: 2025/11/29 12:08:46 by nbuquet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <dirent.h>            /* opendir, readdir, closedir */
# include <fcntl.h>             /* open */
# include <readline/history.h>  /* add_history, rl_clear_history */
# include <readline/readline.h> /* readline, rl_on_new_line, rl_replace_line */
# include <signal.h>            /* signal, sigaction, kill */
# include <stdio.h>             /* printf, perror */
# include <stdlib.h>            /* malloc, free, getenv, exit */
# include <string.h>            /* strerror */
# include <sys/stat.h>          /* stat, lstat, fstat */
# include <sys/wait.h>          /* wait, waitpid, wait3, wait4 */
# include <termios.h>           /* tcsetattr, tcgetattr */
# include <unistd.h>            /* fork, pipe, dup, dup2, execve, access, chdir,
	write, read, close, isatty, ttyname, ttyslot, getcwd */

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

char			*resolve_path(const char *cmd, char **envp);
int				is_absolute(const char *cmd);
char			*resolve_absolute(const char *cmd);
char			*resolve_cmd(const char *cmd, char **envp);

int				process_heredocs(t_exec *exec);

int				is_builtin(char *cmd);
int				is_builtin_statefull(t_exec *exec);

int				process_redirs(t_command *cmd);

#endif
