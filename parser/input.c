#include "../include/minishell.h"

// Función auxiliar para imprimir la estructura t_exec
void print_exec(t_exec *exec)
{
    size_t i, j;

    printf("Pipeline:\n");
    printf("Number of commands: %zu\n", exec->cmd_c);
    for (i = 0; i < exec->cmd_c; i++)
    {
        t_command *cmd = &exec->cmds[i];
        printf("  Command %zu:\n", i);
        printf("    Role: %d\n", cmd->role);
        printf("    Arguments (argv):\n");
        j = 0;
        while (cmd->argv && cmd->argv[j])
        {
            printf("      [%s]\n", cmd->argv[j]);
            j++;
        }
        printf("    Redirections (ios):\n");
        for (j = 0; j < cmd->io_c; j++)
        {
            t_iospec *io = &cmd->ios[j];
            printf("      Type: %d, Arg: %s, Expand: %d\n", io->type, io->arg, io->expand);
        }
    }
}

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
        free(temp_lexer);
    }
}

void free_tokenized_line(t_token *tokenized_line)
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
}

/*void free_tokenized_line(t_token *tokenized_line)
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
}*/

void handle_input(char *line, t_exec_ctx ctx)
{
    t_lexer *lexed_line;
    t_token *tokenized_line;
    t_exec *exec;

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

    expand(tokenized_line, &ctx);
    // Llamar a fill_exec y luego imprimir la estructura t_exec
    exec = fill_exec(tokenized_line);
    if (exec)
    {
        print_exec(exec);
        // Liberar memoria de la estructura t_exec después de imprimir
        //free(exec->cmds);
        //free(exec);
    }

    free_lexed_line(lexed_line);
    free_tokenized_line(tokenized_line);
}