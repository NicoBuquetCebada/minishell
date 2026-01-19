#include "minishell.h"
#include "parser.h"
#include "executor.h"

typedef struct s_it
{
	t_token		*t;
	t_tokenpart	*p;
}	t_it;

static int	is_redir(t_tokentype t)
{
	return (t == REDIR_IN || t == REDIR_OUT || t == APPEND || t == HEREDOC);
}

static t_iotype	map_iotype(t_tokentype t)
{
	if (t == REDIR_IN) return (IO_FILE_IN);
	if (t == REDIR_OUT) return (IO_FILE_TRUNC);
	if (t == APPEND) return (IO_FILE_APPEND);
	return (IO_FILE_HEREDOC);
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
		return ((void)(it->p = it->p->next));
	it->t = it->t->next;
	while (it->t && !it->t->parts)
		it->t = it->t->next;
	it->p = (it->t ? it->t->parts : NULL);
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

static void	set_role(t_command *c, size_t i, size_t total)
{
	if (total == 1) c->role = TAIL;
	else if (i == 0) c->role = HEAD;
	else if (i + 1 == total) c->role = TAIL;
	else c->role = MIDDLE;
}

static void	init_cmd(t_command *c)
{
	c->argv = NULL;
	c->ios = NULL;
	c->io_c = 0;
	c->resolved_path = NULL;
	c->role = HEAD;
}

static void	count_seg(t_it it, size_t *wc, size_t *rc)
{
	*wc = 0;
	*rc = 0;
	while (it.p && it.p->type != PIPE)
	{
		if (is_redir(it.p->type))
		{
			(*rc)++;
			it_next(&it);
			if (it.p) it_next(&it);
			continue ;
		}
		if (it.p->type == WORD)
			(*wc)++;
		it_next(&it);
	}
}

static int	alloc_cmd(t_command *c, size_t wc, size_t rc)
{
	c->argv = (char **)malloc(sizeof(char *) * (wc + 1));
	if (!c->argv)
		return (0);
	c->argv[0] = NULL;
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

static int	fill_seg(t_it *it, t_command *c, size_t wc)
{
	size_t	ai;
	size_t	ri;

	ai = 0;
	ri = 0;
	while (it->p && it->p->type != PIPE)
	{
		if (is_redir(it->p->type))
		{
			c->ios[ri].type = map_iotype(it->p->type);
			it_next(it);
			c->ios[ri].arg = ft_strdup(it->p->value);
			c->ios[ri].expand = (c->ios[ri].type != IO_FILE_HEREDOC);
			ri++; it_next(it); continue ;
		}
		if (it->p->type == WORD && ai < wc)
			c->argv[ai++] = ft_strdup(it->p->value);
		it_next(it);
	}
	c->argv[ai] = NULL;
	return (1);
}

static void	free_cmd_arrays(t_command *c)
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

static void	free_exec_partial(t_exec *e, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
		free_cmd_arrays(&e->cmds[i++]);
	free(e->cmds);
	free(e);
}

t_exec	*fill_exec(t_token *tokens)
{
	t_exec	*e;
	t_it	it;
	size_t	i, wc, rc;

	e = (t_exec *)malloc(sizeof(t_exec));
	if (!e)
        return (NULL);
	e->cmd_c = count_cmds(tokens);
	e->cmds = (t_command *)malloc(sizeof(t_command) * e->cmd_c);
	if (!e->cmds) return (free(e), NULL);
	i = 0;
	it_init(&it, tokens);
	while (i < e->cmd_c)
	{
		init_cmd(&e->cmds[i]);
		set_role(&e->cmds[i], i, e->cmd_c);
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
