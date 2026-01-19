#include "minishell.h"
#include <stdlib.h>

static size_t	ms_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s && s[i])
		i++;
	return (i);
}

static char	*ms_join(const char *a, const char *b)
{
	size_t	la;
	size_t	lb;
	size_t	i;
	char	*r;

	la = ms_strlen(a);
	lb = ms_strlen(b);
	r = (char *)malloc(la + lb + 1);
	if (!r)
		return (NULL);
	i = 0;
	while (a && *a)
		r[i++] = *a++;
	while (b && *b)
		r[i++] = *b++;
	r[i] = '\0';
	return (r);
}

static void	ms_cut_next(t_tokenpart *cur)
{
	t_tokenpart	*dead;

	dead = cur->next;
	cur->next = dead->next;
	free(dead->value);
	free(dead);
}

static int	ms_merge_one(t_tokenpart *cur)
{
	char	*merged;

	merged = ms_join(cur->value, cur->next->value);
	if (!merged)
		return (0);
	free(cur->value);
	cur->value = merged;
	ms_cut_next(cur);
	return (1);
}

static int	ms_merge_parts(t_tokenpart *p)
{
	while (p)
	{
		if (p->type == WORD)
		{
			while (p->next && p->next->type == WORD)
				if (!ms_merge_one(p))
					return (0);
		}
		p = p->next;
	}
	return (1);
}

void	merge_words(t_token *toks)
{
	while (toks)
	{
		if (toks->parts && !ms_merge_parts(toks->parts))
			return;
		toks = toks->next;
	}
}
