/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irrevuel <irrevuel@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 00:10:37 by irrevuel          #+#    #+#             */
/*   Updated: 2026/01/26 20:00:19 by irrevuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"
#include "parser.h"

int	redir_iotype(t_tokentype t)
{
	if (t == REDIR_IN)
		return (IO_FILE_IN);
	if (t == REDIR_OUT)
		return (IO_FILE_TRUNC);
	if (t == APPEND)
		return (IO_FILE_APPEND);
	if (t == HEREDOC)
		return (IO_FILE_HEREDOC);
	return (-1);
}

void	it_init(t_it *it, t_token *t)
{
	it->t = t;
	it->p = NULL;
	while (it->t && !it->t->parts)
		it->t = it->t->next;
	if (it->t)
		it->p = it->t->parts;
}

void	it_next(t_it *it)
{
	if (!it->t || !it->p)
		return ;
	if (it->p->next)
	{
		it->p = it->p->next;
		return ;
	}
	it->t = it->t->next;
	while (it->t && !it->t->parts)
		it->t = it->t->next;
	it->p = NULL;
	if (it->t)
		it->p = it->t->parts;
}

void	init_cmd(t_command *c, size_t i, size_t total)
{
	c->argv = NULL;
	c->ios = NULL;
	c->io_c = 0;
	c->resolved_path = NULL;
	if (total == 1 || i + 1 == total)
		c->role = TAIL;
	else if (i == 0)
		c->role = HEAD;
	else
		c->role = MIDDLE;
}

size_t	count_cmds(t_token *tokens)
{
	t_it	it;
	size_t	c;

	if (!tokens)
		return (0);
	c = 1;
	it_init(&it, tokens);
	while (it.p)
	{
		if (it.p->type == PIPE)
			c++;
		it_next(&it);
	}
	return (c);
}
