#include "../include/minishell.h"

t_tokentype set_type(t_lexpart *part)
{
    if (part->type == IN_SQUOTE || part->type == IN_DQUOTE)
        return WORD;
    if (ft_strncmp(part->value, "|", 1) == 0)
        return PIPE;
    if (ft_strncmp(part->value, "<", 2) == 0)
        return REDIR_IN;
    if (ft_strncmp(part->value, ">", 2) == 0)
        return REDIR_OUT;
    if (ft_strncmp(part->value, "<<", 2) == 0)
        return HEREDOC;
    if (ft_strncmp(part->value, ">>", 2) == 0)
        return APPEND;
    return WORD; 
}

t_token *tokenizer(t_lexer *lexed_line)
{
    t_token *tokens = NULL;
    t_token *aux;
    t_lexpart *current_part;

    add_token(&tokens);
    aux = tokens;

    while (lexed_line)
    {
        current_part = lexed_line->parts;
        while (current_part)
        {
            add_token_part(aux, set_type(current_part), current_part->value, current_part->type);
            current_part = current_part->next;
        }
        lexed_line = lexed_line->next;
        if (lexed_line)
        {
            add_token(&tokens);
            aux = aux->next;
        }
    }
    return tokens;
}