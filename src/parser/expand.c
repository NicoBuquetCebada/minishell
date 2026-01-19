#include "minishell.h"
#include "parser.h"
#include "executor.h"

/*
Devuelve una cadena NUEVA con el valor de la variable.
Si no existe, devuelve NULL (se tratará como "").
*/
char    *get_env_value(const char *key, t_exec_ctx *ctx)
{
    int     i;
    char    *env;
    char    *sep;
    size_t  key_len;

    if (ft_strncmp(key, "?", 2) == 0)
        return (ft_itoa(ctx->last_status));
    key_len = ft_strlen(key);
    i = 0;
    while (ctx->envp[i])
    {
        env = ctx->envp[i];
        sep = ft_strchr(env, '=');
        if (sep && (size_t)(sep - env) == key_len
            && ft_strncmp(env, key, key_len) == 0)
            return (ft_strdup(sep + 1));
        i++;
    }
    return (NULL);
}

/*
Concatena text al final de *result, liberando el antiguo *result.
*/
void    append_text(char **result, const char *text)
{
    char    *temp;

    if (!text)
        return ;
    if (*result)
    {
        temp = ft_strjoin(*result, text);
        free(*result);
        *result = temp;
    }
    else
        *result = ft_strdup(text);
}

static int  is_var_start(char c)
{
    return (ft_isalpha((unsigned char)c) || c == '_');
}

/*
Maneja un '$' en w[i]. Devuelve el nuevo índice i tras la expansión.
*/
static int  expand_var(const char *w, int i, t_exec_ctx *ctx, char **res)
{
    char    *key;
    char    *val;
    int     start;

    i++;                        // saltar '$'
    if (!w[i])
    {
        append_text(res, "$");
        return (i);
    }
    if (w[i] == '?')
    {
        val = get_env_value("?", ctx);
        if (val)
        {
            append_text(res, val);
            free(val);
        }
        return (i + 1);
    }
    if (!is_var_start(w[i]))
    {
        append_text(res, "$");
        return (i);
    }
    start = i;
    while (w[i] && (ft_isalnum((unsigned char)w[i]) || w[i] == '_'))
        i++;
    key = ft_substr(w, start, i - start);
    if (!key)
        return (i);
    val = get_env_value(key, ctx);
    free(key);
    if (val)
    {
        append_text(res, val);
        free(val);
    }
    return (i);
}

/*
Copia literal desde w[i] hasta el próximo '$' o fin.
*/
static int  expand_literal(const char *w, int i, char **res)
{
    int     start;
    char    *lit;

    start = i;
    while (w[i] && w[i] != '$')
        i++;
    lit = ft_substr(w, start, i - start);
    if (lit)
    {
        append_text(res, lit);
        free(lit);
    }
    return (i);
}

/*
** Expande una palabra (sin comillas) usando ctx.
*/
char    *expand_word(const char *w, t_exec_ctx *ctx)
{
    char    *res;
    int     i;

    res = NULL;
    i = 0;
    while (w[i])
    {
        if (w[i] == '$')
            i = expand_var(w, i, ctx, &res);
        else
            i = expand_literal(w, i, &res);
    }
    if (!res)
        res = ft_strdup("");
    return (res);
}

/*
Recorre todos los tokens y expande solo WORD que no vienen de comillas simples.
*/
void    expand(t_token *tokens, t_exec_ctx *ctx)
{
    t_token     *cur;
    t_tokenpart *part;
    char        *expanded;

    cur = tokens;
    while (cur)
    {
        part = cur->parts;
        while (part)
        {
            if (part->type == WORD && part->origin != IN_SQUOTE)
            {
                expanded = expand_word(part->value, ctx);
                if (expanded)
                {
                    free(part->value);
                    part->value = expanded;
                }
            }
            part = part->next;
        }
        cur = cur->next;
    }
    merge_words(tokens); 
}
