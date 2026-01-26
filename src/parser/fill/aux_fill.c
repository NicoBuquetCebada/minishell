/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux_fill.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irrevuel <irrevuel@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 19:05:41 by irrevuel          #+#    #+#             */
/*   Updated: 2026/01/26 20:00:37 by irrevuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"
#include "parser.h"

int	count_seg(t_it it, size_t *wc, size_t *rc)
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
			if (!it.p)
				return (0);
			it_next(&it);
		}
		else
		{
			if (it.p->type == WORD)
				(*wc)++;
			it_next(&it);
		}
	}
	return (1);
}

int	alloc_cmd(t_command *c, size_t wc, size_t rc)
{
	size_t	i;

	c->argv = (char **)malloc(sizeof(char *) * (wc + 1));
	if (!c->argv)
		return (0);
	i = 0;
	while (i < wc + 1)
		c->argv[i++] = NULL;
	c->ios = NULL;
	c->io_c = rc;
	if (!rc)
		return (1);
	c->ios = (t_iospec *)malloc(sizeof(t_iospec) * rc);
	if (!c->ios)
		return (free(c->argv), c->argv = NULL, 0);
	return (1);
}

int	fill_redir(t_it *it, t_command *c, size_t *ri)
{
	int	r;

	r = redir_iotype(it->p->type);
	c->ios[*ri].type = (t_iotype)r;
	it_next(it);
	if (!it->p)
		return (0);
	c->ios[*ri].arg = ft_strdup(it->p->value);
	if (!c->ios[*ri].arg)
		return (0);
	c->ios[*ri].expand = 0;
	if (r == IO_FILE_HEREDOC && it->p->origin == IN_DEFAULT)
		c->ios[*ri].expand = 1;
	(*ri)++;
	it_next(it);
	return (1);
}

int	fill_word(t_it *it, t_command *c, size_t *ai, size_t wc)
{
	if (it->p->type == WORD && *ai < wc)
	{
		c->argv[*ai] = ft_strdup(it->p->value);
		if (!c->argv[*ai])
			return (0);
		(*ai)++;
	}
	it_next(it);
	return (1);
}

int	fill_seg(t_it *it, t_command *c, size_t wc)
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
		{
			if (!fill_redir(it, c, &ri))
				return (0);
		}
		else if (!fill_word(it, c, &ai, wc))
			return (0);
	}
	c->argv[ai] = NULL;
	return (1);
}
