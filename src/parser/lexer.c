#include "minishell.h"
#include "parser.h"

int is_special(char c) 
{
    return (c == '|' || c == '<' || c == '>');
}

static int handle_quotes(char *line, int i, t_lexer *tok, t_lexer_type mode)
{
    int start;
    char quote;

    if (mode == IN_SQUOTE)
        quote = '\'';
    else
        quote = '"';

    start = ++i;
    while (line[i] && line[i] != quote)
        i++;

    if (!line[i])
    {
        printf("Error: open quote %c\n", quote);
        return -1;
    }
    add_part(tok, mode, ft_substr(line, start, i - start));

    if (line[i] == quote)
        i++;

    return i;
}

static int handle_special_or_text(char *line, int i, t_lexer *tok)
{
    int start = i;

    if (line[i] && is_special(line[i]))
    {
        if ((line[i] == '<' && line[i + 1] == '<')
            || (line[i] == '>' && line[i + 1] == '>'))
        {
            add_part(tok, IN_DEFAULT, ft_substr(line, i, 2));
            return i + 2;
        }
        /*if (line[i] == '|' && line[i + 1] == '|')
        {
            printf("Syntax error: unexpected token '|'\n");
            return -1;
        }*/
        add_part(tok, IN_DEFAULT, ft_substr(line, i, 1));
        return i + 1;
    }

    while (line[i] && !isspace((unsigned char)line[i]) 
        && line[i] != '\'' && line[i] != '"' 
        && !is_special(line[i]))
        i++;

    if (i > start)
        add_part(tok, IN_DEFAULT, ft_substr(line, start, i - start));

    return i;
}

int handle_mode(char *line, int i, t_lexer *tok, t_lexer_type mode)
{
    if (mode == IN_SQUOTE || mode == IN_DQUOTE)
        return handle_quotes(line, i, tok, mode);
    return handle_special_or_text(line, i, tok);
}

static int process_token(char *line, int i, t_lexer *nodes, t_lexer *aux)
{
    if (line[i] == '\'')
        i = handle_mode(line, i, aux, IN_SQUOTE);
    else if (line[i] == '"')
        i = handle_mode(line, i, aux, IN_DQUOTE);
    else
        i = handle_mode(line, i, aux, IN_DEFAULT);

    if (i == -1)
    {
        free_lexed_line(nodes);
        return -1;
    }
    return i;
}

static int skip_spaces(char *line, int i, t_lexer **nodes, t_lexer **aux)
{
    while (line[i] && isspace((unsigned char)line[i]))
        i++;

    if (line[i])
    {
        add_node(nodes);
        *aux = (*aux)->next;
    }
    return i;
}

t_lexer *lexer(char *line)
{
    t_lexer *nodes = NULL;
    t_lexer *aux;
    int i;

    if (!line)
        return NULL;
    add_node(&nodes);
    aux = nodes;
    i = 0;

    while (line[i])
    {
        if (isspace((unsigned char)line[i]))
            i = skip_spaces(line, i, &nodes, &aux);
        else
        {
            i = process_token(line, i, nodes, aux);
            if (i == -1)
                return NULL;
        }
    }
    return nodes;
}


