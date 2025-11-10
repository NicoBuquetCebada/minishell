/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbuquet- <nbuquet-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 21:17:13 by nbuquet-          #+#    #+#             */
/*   Updated: 2025/11/10 20:41:23 by nbuquet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

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

typedef enum e_io_src
{
	IO_INHERIT,		/* normal STDIN/STDOUT */
	IO_PIPE_IN, 	/* reads the previous pipe */
	IO_PIPE_OUT, 	/* writes in the next pipe */
	IO_FILE_IN, 	/* < file */
	IO_FILE_TRUNC, 	/* > file (truncate/create) */
	IO_FILE_APPEND,	/* >> file (append/create) */
	IO_FILE_HEREDOC	/* stdin from heredoc tmpfile */
}	t_io_src;

typedef struct s_iospec
{
	t_io_src	in_src;		/* where the input comes from  */
	char		*in_path;	/* path (if IO_FILE_IN or IO_FILE_HEREDOC) */
	t_io_src	out_src;	/* where the output goes */
	char		*out_path;	/* path (if FILE_TRUNC or APPEND) */
} t_iospec;

typedef struct s_command
{
	char				**argv;
	t_iospec			io;
	t_role				role;
	int					has_heredoc; 	/* 1 if had heredoc */
	char				*resolved_path;	/* path optionaly filled by the executor */
	struct s_command	*next;
}	t_command;

typedef struct s_exec
{
	t_command	*cmds;	/* command list */
	int			cmd_c;	/* number of commands */
}	t_exec;

/* Execution context carried by the shell main loop */
typedef struct s_exec_ctx
{
	char			**envp;      	/* environment as a modifiable array */
	int				last_status;	/* last pipeline exit status (used to expand $?) */
	int				interactive; 	/* 1 if running on an interactive terminal, 0 otherwise */
}	t_exec_ctx;

#endif
