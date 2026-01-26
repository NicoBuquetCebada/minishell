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

/*
** Lexer state for a fragment of input:
**   IN_DEFAULT -> Unquoted text
**   IN_SQUOTE  -> Inside single quotes ('')
**   IN_DQUOTE  -> Inside double quotes ("")
**
** Used to track how a fragment was produced, mainly to decide
** if expansions are allowed or not.
*/
typedef enum e_lexer_type
{
	IN_DEFAULT,
	IN_SQUOTE,
	IN_DQUOTE
}						t_lexer_type;

/*
** A fragment inside a single logical lexer token.
**
** Example:
**   hello'$USER'world
** is split into multiple lexparts with different lexer states.
**
** - type  : lexer state that produced this fragment
** - value : raw string value of the fragment
** - next  : next fragment belonging to the same token
*/
typedef struct s_lexpart
{
	t_lexer_type		type;
	char				*value;
	struct s_lexpart	*next;
}						t_lexpart;

/*
** Lexer node.
**
** Represents one lexed token before final token classification.
** It is composed of one or more lexparts.
**
** - parts : list of fragments composing this lexer token
** - next  : next lexer node in the input stream
*/
typedef struct s_lexer
{
	t_lexpart			*parts;
	struct s_lexer		*next;
}						t_lexer;

/*
** Token types produced after lexical analysis.
**
** - WORD      : normal word / argument
** - PIPE      : |
** - REDIR_IN  : <
** - REDIR_OUT : >
** - HEREDOC   : <<
** - APPEND    : >>
*/
typedef enum e_tokentype
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	HEREDOC,
	APPEND
}						t_tokentype;

/*
** A fragment inside a token.
**
** Similar to lexparts, but at the token stage.
**
** - value  : string value of this part
** - type   : token type associated with this part
** - origin : lexer state where this part originated
** - next   : next part in the same token
*/
typedef struct s_tokenpart
{
	char				*value;
	t_tokentype			type;
	t_lexer_type		origin;
	struct s_tokenpart	*next;
}						t_tokenpart;

/*
** Token node.
**
** Represents one final token in the token stream.
**
** - parts : list of parts composing the token
** - next  : next token in sequence
*/
typedef struct s_token
{
	t_tokenpart			*parts;
	struct s_token		*next;
}						t_token;

/*
** Command metadata helper.
**
** Used to precompute information in order to avoid reallocations
** when building execution structures.
**
** - argc        : number of arguments for the command
** - redir_count : number of redirections
*/
typedef struct s_cmd_info
{
	size_t				argc;
	size_t				redir_count;
}						t_cmd_info;

/*
** Token iterator helper.
**
** Used to iterate through the token stream and their internal parts.
**
** - t : current token
** - p : current token part inside the token
*/
typedef struct s_it
{
	t_token				*t;
	t_tokenpart			*p;
}						t_it;

/* FUNCTIONS */
void					handle_input(char *line, t_exec_ctx *ctx);
t_lexer					*lexer(char *line);
void					add_node(t_lexer **list);
void					add_part(t_lexer *tok, t_lexer_type type, char *value);
t_lexer					*new_node(void);
t_lexpart				*new_part(t_lexer_type type, char *value);
t_token					*tokenizer(t_lexer *lexed);
void					free_lexed_line(t_lexer *lexed_line);
t_token					*new_token(void);
t_tokenpart				*new_token_part(t_tokentype type, char *value,
							t_lexer_type origin);
void					add_token(t_token **list);
void					add_token_part(t_token *tok, t_tokentype type,
							char *value, t_lexer_type origin);
t_tokentype				set_type(t_lexpart *part);
void					expand(t_token *tokens, t_exec_ctx *ctx);
int						validate_tokens(t_token *tokens);
t_exec					*fill_exec(t_token *tokens);
void					print_lexed_line(t_lexer *lexed_line);
void					print_exec(t_exec *exec);
void					print_tokenized_line(t_token *tokenized_line);
void					merge_words(t_token *toks);
int						process_heredocs(t_exec *exec, t_exec_ctx *ctx);
char					*expand_word(const char *w, t_exec_ctx *ctx);
int						handle_mode(char *line, int i, t_lexer *tok,
							t_lexer_type mode);
int						is_var_start(char c);
void					append_text(char **result, const char *text);
char					*get_env_value(const char *key, t_exec_ctx *ctx);
void					free_lexed_line(t_lexer *lexed_line);
void					free_tokenized_line(t_token *tokenized_line);
void					free_exec(t_exec *e);
void					cleanup_input(t_lexer *lex, t_token *tok, t_exec *exec);
int						is_delim(char *line, char *delim);
int						write_ln(int fd, char *s);
int						redir_iotype(t_tokentype t);
void					it_init(t_it *it, t_token *t);
void					it_next(t_it *it);
void					init_cmd(t_command *c, size_t i, size_t total);
size_t					count_cmds(t_token *tokens);
int						count_seg(t_it it, size_t *wc, size_t *rc);
int						alloc_cmd(t_command *c, size_t wc, size_t rc);
int						fill_redir(t_it *it, t_command *c, size_t *ri);
int						fill_word(t_it *it, t_command *c, size_t *ai,
							size_t wc);
int						fill_seg(t_it *it, t_command *c, size_t wc);
void					free_one_cmd(t_command *c);
void					free_exec_partial(t_exec *e, size_t n);
int						init_exec(t_exec **e, t_token *tokens);
int						build_one_cmd(t_exec *e, t_it *it, size_t i);
t_exec					*fill_exec(t_token *tokens);

#endif
