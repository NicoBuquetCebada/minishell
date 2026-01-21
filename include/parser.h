/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbuquet- <nbuquet-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 21:17:13 by nbuquet-          #+#    #+#             */
/*   Updated: 2025/11/02 23:41:39 by nbuquet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "executor.h"

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
void print_lexed_line(t_lexer *lexed_line);
void print_exec(t_exec *exec);
void print_tokenized_line(t_token *tokenized_line);
void	merge_words(t_token *toks);
int	process_heredocs(t_exec *exec, t_exec_ctx *ctx);
char    *expand_word(const char *w, t_exec_ctx *ctx);

#endif
