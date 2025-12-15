#include "../include/minishell.h"

static int check_pipe_edges(t_token *tk)
{
    if (tk && tk->parts && tk->parts->type == PIPE)
    {
        printf("Syntax error: unexpected token '|'\n");
        return 0;
    }
    while (tk && tk->next)
        tk = tk->next;
    if (tk && tk->parts && tk->parts->type == PIPE)
    {
        printf("Syntax error: unexpected token '|'\n");
        return 0;
    }
    return 1;
}


static int check_consecutive_pipes(t_token *tk)
{
    t_tokenpart *p;

    while (tk)
    {
        p = tk->parts;
        while (p)
        {
            if (p->type == PIPE && p->next && p->next->type == PIPE)
            {
                printf("Syntax error: unexpected token '|'\n");
                return 0;
            }
            p = p->next;
        }
        tk = tk->next;
    }
    return 1;
}


static int check_redirections(t_token *token)
{
    t_tokenpart *p;

    while (token)
    {
        p = token->parts;
        while (p)
        {
            if (p->type == REDIR_IN || p->type == REDIR_OUT
             || p->type == APPEND   || p->type == HEREDOC)
            {
                if (p->next && p->next->type == WORD)
                {
                    p = p->next;
                    continue;
                }
                if (token->next && token->next->parts && token->next->parts->type == WORD)
                {
                    token = token->next;
                    p = token->parts;
                    continue;
                }
                printf("Syntax error: expected WORD after redirection\n");
                return 0;
            }
            p = p->next;
        }
        token = token->next;
    }
    return 1;
}

int validate_tokens(t_token *tokens)
{
    if (!tokens)
        return 1;
    if (!check_pipe_edges(tokens) || !check_consecutive_pipes(tokens) 
        || !check_redirections(tokens))
        return 0;
    return 1;
}