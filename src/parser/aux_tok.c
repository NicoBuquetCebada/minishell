#include "minishell.h"
#include "parser.h"

t_token *new_token(void)
{
    t_token *t;
    
    t = malloc(sizeof(t_token));
    if (!t)
        return NULL;
    t->parts = NULL;
    t->next = NULL;
    return t;
}

t_tokenpart *new_token_part(t_tokentype type, char *value, t_lexer_type origin)
{
    t_tokenpart *part;
    
    part = malloc(sizeof(t_tokenpart));
    if (!part)
        return NULL;
    part->type = type;
    part->value = strdup(value);
    part->origin = origin;
    if (!part->value)
    {
        free(part);
        return NULL;
    }
    part->next = NULL;
    return part;
}

void add_token(t_token **list)
{
    t_token *new;
    t_token *tmp;

    if (!list)
        return;
    new = new_token();
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

void add_token_part(t_token *tok, t_tokentype type, char *value, t_lexer_type origin)
{
    t_tokenpart *new;
    t_tokenpart *tmp;

    if (!tok || !value)
        return;
    new = new_token_part(type, value, origin);
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