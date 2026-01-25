/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irrevuel <irrevuel@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 00:33:59 by irrevuel          #+#    #+#             */
/*   Updated: 2026/01/24 00:34:00 by irrevuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "executor.h"

void	cleanup_input(t_lexer *lex, t_token *tok, t_exec *exec)
{
	if (exec)
		free_exec(exec);
	if (tok)
		free_tokenized_line(tok);
	if (lex)
		free_lexed_line(lex);
}

void	handle_input(char *line, t_exec_ctx *ctx)
{
	t_lexer	*lex;
	t_token	*tok;
	t_exec	*exec;

	lex = NULL;
	tok = NULL;
	exec = NULL;
	lex = lexer(line);
	if (!lex)
		return ;
	tok = tokenizer(lex);
	if (!tok || !validate_tokens(tok))
		return (cleanup_input(lex, tok, exec));
	expand(tok, ctx);
	exec = fill_exec(tok);
	if (!exec)
		return (cleanup_input(lex, tok, exec));
	if (!process_heredocs(exec, ctx))
		return (cleanup_input(lex, tok, exec));
	exec_caller(ctx, exec);
	cleanup_input(lex, tok, exec);
}
