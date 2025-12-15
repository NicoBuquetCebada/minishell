#include "../include/minishell.h"

char *get_env_value(const char *key, t_exec_ctx *ctx)
{
    if (ft_strncmp(key, "?", 2) == 0)
        return ft_itoa(ctx->last_status); // Expandir $?
        //Tengo que ver cuando actualizo ese valor!!
    int i = 0;
    while (ctx->envp[i])
    {
        char *env = ctx->envp[i];
        char *sep = strchr(env, '='); //funcion lib
        if (sep && ft_strncmp(env, key, sep - env) == 0) //función lib
            return ft_strdup(sep + 1); // funcion lib
        i++;
    }
    return NULL;
}

int get_var_name(const char *w, int i, char **key)
{
    int start = i;

    while (w[i] && (ft_isalnum(w[i]) || w[i] == '_'))
        i++;

    *key = ft_substr(w, start, i - start);
    return i;
}

void append_text(char **result, const char *text)
{
    char *temp;

    if (*result)
    {
        temp = ft_strjoin(*result, text);
        free(*result);
        *result = temp;
    }
    else
        *result = ft_strdup(text);
}

int expand_var(const char *w, int i, t_exec_ctx *ctx, char **res)
{
    char *key;
    char *value;
    char *literal;

    i++;
    i = get_var_name(w, i, &key);
    value = get_env_value(key, ctx);
    free(key);
    if (value)
    {
        append_text(res, value);
        free(value);
    }
    else
    {
        literal = ft_substr(w, i - strlen(key) - 1, strlen(key) + 1);
        append_text(res, literal);
        free(literal);
    }
    return i;
}


int expand_literal(const char *w, int i, char **res)
{
    int start;
    char *literal;
    char *temp;

    start = i;
    while (w[i] && w[i] != '$')
        i++;
    literal = ft_substr(w, start, i - start);
    append_text(res, literal);
    free(literal);
    return i;
}

char *expand_word(const char *word, t_exec_ctx *ctx)
{
    char *result;
    int i;

    i = 0;
    result = NULL;
    while (word[i])
    {
        if (word[i] == '$')
            i = expand_var(word, i, ctx, &result);
        else
            i = expand_literal(word, i, &result);
    }
    return result;
}

void expand(t_token *tokens, t_exec_ctx *ctx)
{
    t_token *current;
    t_tokenpart *part;

    current = tokens;
    while (current)
    {
        part = current->parts;
        while (part)
        {
            if (part->type == WORD && part->origin != IN_SQUOTE)
            {
                char *expanded = expand_word(part->value, ctx);
                free(part->value);
                part->value = expanded;
            }
            part = part->next;
        }
        current = current->next;
    }
}