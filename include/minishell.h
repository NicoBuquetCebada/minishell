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

# include "../lib/libft/libft.h"
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
#include <string.h>
#include <ctype.h>


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

// PARSEO //

typedef enum e_lexer_type
{
	IN_DEFAULT,
	IN_SQUOTE,
	IN_DQUOTE
}	t_lexer_type;

// Un fragmento dentro de un mismo token (hola'$USER'hula)

typedef struct s_lexpart
{
	t_lexer_type	type;
	char			*value;
	struct s_lexpart	*next;
}	t_lexpart;

//Nodo del lexer -> debería de tener tipo?

typedef struct s_lexer
{
	t_lexpart		*parts;       
	struct s_lexer	*next; 
}	t_lexer;

//Tokens

typedef enum e_tokentype
{
    WORD,
    PIPE,
    REDIR_IN,  
    REDIR_OUT, 
    HEREDOC, 
    APPEND      
}   t_tokentype;

// mismo concepto que el lexer, pero en tokens

typedef struct s_tokenpart
{
    char                *value;  
	t_tokentype         type;
	t_lexer_type		origin;
    struct s_tokenpart  *next;
}   t_tokenpart;

//nodo del lexer

typedef struct s_token
{
    t_tokenpart         *parts;
    struct s_token      *next;
}   t_token;

//info pra no tener que realocar

typedef struct s_cmd_info
{
    size_t argc;
    size_t redir_count;
} t_cmd_info;

typedef struct s_exec_info
{
    size_t cmd_count;
    t_cmd_info *cmds;
} t_exec_info;


// funciones en plan cutre xd

void handle_input(char *line,  t_exec_ctx ctx);
t_lexer *lexer(char *line);
void	add_node(t_lexer **list);
void	add_part(t_lexer *tok, t_lexer_type type, char *value);
t_lexer *new_node(void);
t_lexpart *new_part(t_lexer_type type, char *value);
t_token *tokenizer(t_lexer *lexed);
void free_lexed_line(t_lexer *lexed_line);
t_token *new_token(void);
t_tokenpart *new_token_part(t_tokentype type, char *value, t_lexer_type origin);
void add_token(t_token **list);
void add_token_part(t_token *tok, t_tokentype type, char *value, t_lexer_type origin);
t_tokentype set_type(t_lexpart *part);
//void merge_and_expand(t_token *tokenized_line, t_exec_ctx *ctx);
void expand(t_token *tokens, t_exec_ctx *ctx);
int validate_tokens(t_token *tokens);
t_exec *fill_exec(t_token *tokens);


#endif
