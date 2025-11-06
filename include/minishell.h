/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbuquet- <nbuquet-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 21:17:13 by nbuquet-          #+#    #+#             */
/*   Updated: 2025/11/02 23:41:39 by nbuquet-         ###   ########.fr       */
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

typedef enum e_redir_type
{
	R_IN,           /* <  : stdin from file */
	R_OUT_TRUNC,    /* >  : stdout to file (truncate) */
	R_OUT_APPEND,	/* >> : stdout to file (append) */
	R_HEREDOC       /* << : stdin from heredoc */
}	t_redir_type;

typedef struct s_redir
{
	char			*file;          /* target file for <, >, >> (NULL for heredoc) */
	t_redir_type	type;           /* redirection kind */
	char			*heredoc_delim;	/* delimiter token, as given by parser */
	int				heredoc_expand; /* 1: expand $ in heredoc, 0: do not expand */
	int				heredoc_fd;     /* prepared read-end fd (executor can fill) */
	struct s_redir	*next;
}	t_redir;

typedef struct s_command
{
	char			**argv;
	t_redir			*redirs;        /* linked list of redirections for this command */
	int				redir_count;    /* convenience counter for executor loops */
	char			*resolved_path;	/* path optionaly filled by the executor */
	struct s_command *next;
}	t_command;

/* Execution context carried by the shell main loop */
typedef struct s_exec_ctx
{
	char			**envp;      	/* environment as a modifiable array */
	int				last_status;	/* last pipeline exit status (used to expand $?) */
	int				interactive; 	/* 1 if running on an interactive terminal, 0 otherwise */
}	t_exec_ctx;

#endif
