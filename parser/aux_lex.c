#include "../include/minishell.h"

t_lexpart *new_part(t_lexer_type type, char *value)
{
    t_lexpart *part;
	
	part = malloc(sizeof(t_lexpart));
    if (!part)
	{
		free(value);
		return NULL;
	}
    part->type = type;
    part->value = value;
    if (!part->value)
    {
        free(part);
        return NULL;
    }
    part->next = NULL;
    return part;
}

t_lexer *new_node(void)
{
	t_lexer *t;
	t = malloc(sizeof(t_lexer));
	if (!t)
		return NULL;
	t->parts = NULL;
	t->next = NULL;
	return t;
}

void	add_part(t_lexer *tok, t_lexer_type type, char *value)
{
	t_lexpart *new;
	t_lexpart *tmp;
	
	if (!tok || !value)
		return;
	new = new_part(type, value);
	if (!new)
		return;
	if (!tok->parts)
		tok->parts = new;
	else
	{
		tmp = tok->parts;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

void	add_node(t_lexer **list)
{
	t_lexer *tmp;
	t_lexer *new;

	if (!list)
		return;
	new = new_node();
	if (!new)
		return;
	if (!*list)
		*list = new;
	else
	{
		tmp = *list;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}