#include "minishell.h"
#include "parser.h"
#include "executor.h"

static const char	*role_str(t_role r)
{
	if (r == HEAD) return ("HEAD");
	if (r == MIDDLE) return ("MIDDLE");
	return ("TAIL");
}

static const char	*io_str(t_iotype t)
{
	if (t == IO_FILE_IN) return ("<");
	if (t == IO_FILE_TRUNC) return (">");
	if (t == IO_FILE_APPEND) return (">>");
	return ("<<");
}

static void	print_argv(char **argv)
{
	size_t	i;

	i = 0;
	while (argv && argv[i])
	{
		printf("      argv[%zu] = '%s'\n", i, argv[i]);
		i++;
	}
}

static void	print_ios(t_iospec *ios, size_t io_c)
{
	size_t	i;

	i = 0;
	while (ios && i < io_c)
	{
		printf("      io[%zu] = %s arg='%s' expand=%d\n",
			i, io_str(ios[i].type), ios[i].arg, ios[i].expand);
		i++;
	}
}

void	print_exec(t_exec *exec)
{
	size_t	i;

	if (!exec)
		return ;
	printf("EXEC: cmd_c=%zu\n", exec->cmd_c);
	i = 0;
	while (i < exec->cmd_c)
	{
		printf("  CMD %zu (%s)\n", i, role_str(exec->cmds[i].role));
		printf("    argv:\n");
		print_argv(exec->cmds[i].argv);
		printf("    ios: io_c=%zu\n", exec->cmds[i].io_c);
		print_ios(exec->cmds[i].ios, exec->cmds[i].io_c);
		i++;
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

void print_tokenized_line(t_token *tokenized_line)
{
    t_token *current_token = tokenized_line;
    size_t part_index;

    printf("Tokenized Line:\n");
    while (current_token)
    {
        t_tokenpart *current_part = current_token->parts;
        printf("  Token Node:\n");
        part_index = 0;
        while (current_part)
        {
            printf("    Part %zu: Type: %d, Value: [%s]\n", part_index, current_part->type, current_part->value);
            current_part = current_part->next;
            part_index++;
        }
        current_token = current_token->next;
    }
}