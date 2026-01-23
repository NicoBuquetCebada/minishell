/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irrevuel <irrevuel@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 23:57:43 by irrevuel          #+#    #+#             */
/*   Updated: 2026/01/23 23:58:02 by irrevuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

static int	check_pipe_edges(t_token *tok)
{
	t_token		*last;
	t_tokenpart	*p;

	if (!tok)
		return (1);
	if (tok->parts && tok->parts->type == PIPE)
	{
		printf("Syntax error: unexpected token '|'\n");
		return (0);
	}
	last = tok;
	while (last->next)
		last = last->next;
	if (!last->parts)
		return (1);
	p = last->parts;
	while (p->next)
		p = p->next;
	if (p->type == PIPE)
	{
		printf("Syntax error: unexpected token '|'\n");
		return (0);
	}
	return (1);
}

static int	check_consecutive_pipes(t_token *tok)
{
	t_tokenpart	*p;
	int			prev_pipe;

	prev_pipe = 0;
	while (tok)
	{
		p = tok->parts;
		while (p)
		{
			if (p->type == PIPE)
			{
				if (prev_pipe)
				{
					printf("Syntax error: unexpected token '|'\n");
					return (0);
				}
				prev_pipe = 1;
			}
			else
				prev_pipe = 0;
			p = p->next;
		}
		tok = tok->next;
	}
	return (1);
}

static int	check_one_redir(t_token **token, t_tokenpart **p)
{
	if ((*p)->next && (*p)->next->type == WORD)
	{
		*p = (*p)->next;
		return (1);
	}
	if ((*token)->next && (*token)->next->parts
		&& (*token)->next->parts->type == WORD)
	{
		*token = (*token)->next;
		*p = (*token)->parts;
		return (1);
	}
	printf("Syntax error: expected WORD after redirection\n");
	return (0);
}

static int	check_redirections(t_token *token)
{
	t_tokenpart	*p;

	while (token)
	{
		p = token->parts;
		while (p)
		{
			if (p->type == REDIR_IN || p->type == REDIR_OUT || p->type == APPEND
				|| p->type == HEREDOC)
				if (!check_one_redir(&token, &p))
					return (0);
			p = p->next;
		}
		token = token->next;
	}
	return (1);
}

int	validate_tokens(t_token *tokens)
{
	if (!tokens)
		return (1);
	if (!check_pipe_edges(tokens) || !check_consecutive_pipes(tokens)
		|| !check_redirections(tokens))
		return (0);
	return (1);
}
