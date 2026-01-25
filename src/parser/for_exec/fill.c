/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irrevuel <irrevuel@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 00:10:37 by irrevuel          #+#    #+#             */
/*   Updated: 2026/01/24 14:03:22 by irrevuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"
#include "parser.h"

static int	redir_iotype(t_tokentype t)
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

static void	it_init(t_it *it, t_token *t)
{
	it->t = t;
	it->p = NULL;
	while (it->t && !it->t->parts)
		it->t = it->t->next;
	if (it->t)
		it->p = it->t->parts;
}

static void	it_next(t_it *it)
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
	if (it->t)
		it->p = it->t->parts;
	else
		it->p = NULL;
}

static void	init_cmd(t_command *c, size_t i, size_t total)
{
	c->argv = NULL;
	c->ios = NULL;
	c->io_c = 0;
	c->resolved_path = NULL;
	if (total == 1)
		c->role = TAIL;
	else if (i == 0)
		c->role = HEAD;
	else if (i + 1 == total)
		c->role = TAIL;
	else
		c->role = MIDDLE;
}

static size_t	count_cmds(t_token *tokens)
{
	t_it	it;
	size_t	c;

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

static void	count_seg(t_it it, size_t *wc, size_t *rc)
{
	int	r;

	*wc = 0;
	*rc = 0;
	while (it.p && it.p->type != PIPE)
	{
		r = redir_iotype(it.p->type);
		if (r != -1)
		{
			(*rc)++;
			it_next(&it);
			if (it.p)
				it_next(&it);
		}
		else
		{
			if (it.p->type == WORD)
				(*wc)++;
			it_next(&it);
		}
	}
}

static int	alloc_cmd(t_command *c, size_t wc, size_t rc)
{
	size_t	i;

	c->argv = (char **)malloc(sizeof(char *) * (wc + 1));
	if (!c->argv)
		return (0);
	i = 0;
	while (i < wc + 1)
	{
		c->argv[i] = NULL;
		i++;
	}
	c->ios = NULL;
	c->io_c = rc;
	if (rc)
	{
		c->ios = (t_iospec *)malloc(sizeof(t_iospec) * rc);
		if (!c->ios)
			return (0);
	}
	return (1);
}

static void	fill_one_redir(t_it *it, t_command *c, size_t *ri)
{
	int	r;

	r = 0;
	r = redir_iotype(it->p->type);
	c->ios[*ri].type = (t_iotype)r;
	it_next(it);
	c->ios[*ri].arg = ft_strdup(it->p->value);
	c->ios[*ri].expand = 0;
	if (r == IO_FILE_HEREDOC && it->p->origin == IN_DEFAULT)
		c->ios[*ri].expand = 1;
	(*ri)++;
	it_next(it);
}

static void	fill_one_word(t_it *it, t_command *c, size_t *ai, size_t wc)
{
	if (it->p->type == WORD && *ai < wc)
	{
		c->argv[*ai] = ft_strdup(it->p->value);
		(*ai)++;
	}
	it_next(it);
}

static int	fill_seg(t_it *it, t_command *c, size_t wc)
{
	size_t	ai;
	size_t	ri;
	int		r;

	ai = 0;
	ri = 0;
	while (it->p && it->p->type != PIPE)
	{
		r = redir_iotype(it->p->type);
		if (r != -1)
			fill_one_redir(it, c, &ri);
		else
			fill_one_word(it, c, &ai, wc);
	}
	c->argv[ai] = NULL;
	return (1);
}

static void	free_one_cmd(t_command *c)
{
	size_t	i;

	i = 0;
	while (c->argv && c->argv[i])
	{
		free(c->argv[i]);
		i++;
	}
	free(c->argv);
	i = 0;
	while (c->ios && i < c->io_c)
	{
		free(c->ios[i].arg);
		i++;
	}
	free(c->ios);
	free(c->resolved_path);
}

static void	free_exec_partial(t_exec *e, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		free_one_cmd(&e->cmds[i]);
		i++;
	}
	free(e->cmds);
	free(e);
}

t_exec	*fill_exec(t_token *tokens)
{
	t_exec	*e;
	t_it	it;
	size_t	i;
	size_t	wc;
	size_t	rc;

	e = NULL;
	e = (t_exec *)malloc(sizeof(t_exec));
	if (!e)
		return (NULL);
	e->cmd_c = count_cmds(tokens);
	e->cmds = (t_command *)malloc(sizeof(t_command) * e->cmd_c);
	if (!e->cmds)
	{
		free(e);
		return (NULL);
	}
	it_init(&it, tokens);
	i = 0;
	while (i < e->cmd_c)
	{
		init_cmd(&e->cmds[i], i, e->cmd_c);
		count_seg(it, &wc, &rc);
		if (!alloc_cmd(&e->cmds[i], wc, rc))
			return (free_exec_partial(e, i), NULL);
		if (!fill_seg(&it, &e->cmds[i], wc))
			return (free_exec_partial(e, i + 1), NULL);
		if (it.p && it.p->type == PIPE)
			it_next(&it);
		i++;
	}
	return (e);
}

