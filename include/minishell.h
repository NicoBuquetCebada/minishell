/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbuquet- <nbuquet-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 21:17:13 by nbuquet-          #+#    #+#             */
/*   Updated: 2025/11/23 19:40:49 by nbuquet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <stdio.h>		        /* printf, perror */
# include <stdlib.h>       		/* malloc, free, getenv, exit */
# include <unistd.h>    		/* fork, pipe, dup, dup2, execve, access, chdir, write, read, close, isatty, ttyname, ttyslot, getcwd */
# include <fcntl.h>   		   	/* open */
# include <dirent.h>       		/* opendir, readdir, closedir */
# include <string.h>       		/* strerror */
# include <sys/stat.h>  		/* stat, lstat, fstat */
# include <sys/wait.h>    		/* wait, waitpid, wait3, wait4 */
# include <signal.h>      		/* signal, sigaction, kill */
# include <termios.h>    		/* tcsetattr, tcgetattr */
# include <readline/readline.h>	/* readline, rl_on_new_line, rl_replace_line, rl_redisplay */
# include <readline/history.h>  /* add_history, rl_clear_history */

typedef enum e_role
{
	ROLE_HEAD,
	ROLE_MIDDLE,
	ROLE_TAIL 
} t_role;

/*
** Tipo de redirección:
**   IO_FILE_IN      ->  < file
**   IO_FILE_TRUNC   ->  > file
**   IO_FILE_APPEND  ->  >> file
**   IO_FILE_HEREDOC ->  << delimiter (heredoc)
**
** NOTA:
** - No metemos aquí los pipes ni el "inherit".
**   - La herencia de STDIN/STDOUT viene dada por defecto si no hay
**     redirecciones de ese tipo.
**   - Los pipes los deduces por la posición (role) dentro del pipeline.
*/
typedef enum e_io_type
{
	IO_FILE_IN,
	IO_FILE_TRUNC,
	IO_FILE_APPEND,
	IO_FILE_HEREDOC
}	t_io_type;

/*
** A single redirection specification:
** - type   : redirection type
** - arg    : filename or heredoc delimiter
** - expand : for heredocs only (1 = variables expansion, 0 = no expansion)
*/
typedef struct s_iospec
{
	t_io_type	type;
	char		*arg;
	int			expand;
} t_iospec;

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
	char				**argv;
	t_iospec			*ios;
	int					io_c;
	t_role				role;
	char				*resolved_path;
}	t_command;

/*
** A complete pipeline:
** - cmds  : array of commands
** - cmd_c : number of commands in the pipeline
*/
typedef struct s_exec
{
	t_command	*cmds;
	int			cmd_c;
}	t_exec;

/*
** Execution context stored globally:
** - envp          : modifiable environment array
** - last_status   : last pipeline exit status (used for $? expansion)
** - interactive   : 1 if shell is interactive, 0 otherwise
*/
typedef struct s_exec_ctx
{
	char			**envp;
	int				last_status;
	int				interactive;
}	t_exec_ctx;

char	*resolve_path(const char *cmd, char **envp);
int		is_absolute(const char *cmd);
char	*resolve_absolute(const char *cmd);
char	*resolve_cmd(const char *cmd, char **envp);

#endif
