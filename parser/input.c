#include "../include/minishell.h"

void free_lexed_line(t_lexer *lexed_line)
{ 
    t_lexer *temp_lexer;
    t_lexpart *temp_part;

    while (lexed_line)
    {
        while (lexed_line->parts)
        {
            temp_part = lexed_line->parts;
            lexed_line->parts = lexed_line->parts->next;
            free(temp_part->value);
            free(temp_part);
        }
        temp_lexer = lexed_line;
        lexed_line = lexed_line->next;
        if (temp_lexer)
            free(temp_lexer);
    }
}

/*void free_tokenized_line(t_token *tokenized_line)
{
    t_token *tmp;
    t_tokenpart *p;
    t_tokenpart *next;

    while (tokenized_line)
    {
        tmp = tokenized_line->next;

        p = tokenized_line->parts;
        while (p)
        {
            next = p->next;
            free(p->value);
            free(p);
            p = next;
        }
        free(tokenized_line);
        tokenized_line = tmp;
    }
}*/

void free_tokenized_line(t_token *tokenized_line)
{ 
    t_token *temp_token;
    t_tokenpart *temp_part;

    while (tokenized_line)
    {
        while (tokenized_line->parts)
        {
            temp_part = tokenized_line->parts;
            tokenized_line->parts = tokenized_line->parts->next;
            free(temp_part->value);
            free(temp_part);
        }
        temp_token = tokenized_line;
        tokenized_line = tokenized_line->next;
        free(temp_token);
    }
}

void handle_input(char *line, t_exec_ctx ctx)
{
    t_lexer *lexed_line;
    t_token *tokenized_line;
    //t_exec *exec;

    lexed_line = lexer(line);
    if (!lexed_line)
        return;
    tokenized_line = tokenizer(lexed_line);
    if (!tokenized_line)
    {
        free_lexed_line(lexed_line);
        return;
    }
    if (validate_tokens(tokenized_line) == 0)
    {
        free_lexed_line(lexed_line);
        free_tokenized_line(tokenized_line);
        return;
    }

    /*expand(tokenized_line, &ctx);
    // Llamar a fill_exec y luego imprimir la estructura t_exec
    exec = fill_exec(tokenized_line);
    if (exec)
    {
        print_exec(exec);
        // Liberar memoria de la estructura t_exec después de imprimir
        free(exec->cmds);
        free(exec);
    }*/

    free_lexed_line(lexed_line);
    free_tokenized_line(tokenized_line);
}