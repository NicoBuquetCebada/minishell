/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_fill.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irrevuel <irrevuel@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 19:03:11 by irrevuel          #+#    #+#             */
/*   Updated: 2026/01/26 19:59:59 by irrevuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"
#include "parser.h"

void	free_one_cmd(t_command *c)
{
	size_t	i;

	i = 0;
	while (c->argv && c->argv[i])
		free(c->argv[i++]);
	free(c->argv);
	i = 0;
	while (c->ios && i < c->io_c)
		free(c->ios[i++].arg);
	free(c->ios);
	free(c->resolved_path);
}

void	free_exec_partial(t_exec *e, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
		free_one_cmd(&e->cmds[i++]);
	free(e->cmds);
	free(e);
}

int	init_exec(t_exec **e, t_token *tokens)
{
	*e = NULL;
	*e = (t_exec *)malloc(sizeof(t_exec));
	if (!*e)
		return (0);
	(*e)->cmd_c = count_cmds(tokens);
	(*e)->cmds = NULL;
	(*e)->cmds = (t_command *)malloc(sizeof(t_command) * (*e)->cmd_c);
	if (!(*e)->cmds)
		return (free(*e), *e = NULL, 0);
	return (1);
}

int	build_one_cmd(t_exec *e, t_it *it, size_t i)
{
	t_it	tmp;
	size_t	wc;
	size_t	rc;

	tmp = *it;
	if (!count_seg(tmp, &wc, &rc))
		return (0);
	init_cmd(&e->cmds[i], i, e->cmd_c);
	if (!alloc_cmd(&e->cmds[i], wc, rc))
		return (0);
	if (!fill_seg(it, &e->cmds[i], wc))
		return (0);
	if (it->p && it->p->type == PIPE)
		it_next(it);
	return (1);
}

t_exec	*fill_exec(t_token *tokens)
{
	t_exec	*e;
	t_it	it;
	size_t	i;

	if (!tokens)
		return (NULL);
	if (!init_exec(&e, tokens))
		return (NULL);
	it_init(&it, tokens);
	i = 0;
	while (i < e->cmd_c)
	{
		if (!build_one_cmd(e, &it, i))
			return (free_exec_partial(e, i + 1), NULL);
		i++;
	}
	return (e);
}
