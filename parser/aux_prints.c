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

void print_lexed_line(t_lexer *lexed_line)
{
    t_lexer *current_lexer = lexed_line;
    size_t part_index;

    printf("Lexed Line:\n");
    while (current_lexer)
    {
        t_lexpart *current_part = current_lexer->parts;
        printf("  Lexer Node:\n");
        part_index = 0;
        while (current_part)
        {
            printf("    Part %zu: Type: %d, Value: [%s]\n", part_index, current_part->type, current_part->value);
            current_part = current_part->next;
            part_index++;
        }
        current_lexer = current_lexer->next;
    }
}