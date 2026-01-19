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

#include "minishell.h"
#include <stdlib.h>

static void	free_argv(char **argv)
{
	size_t	i;

	if (!argv)
		return ;
	i = 0;
	while (argv[i])
		free(argv[i++]);
	free(argv);
}

static void	free_ios(t_iospec *ios, size_t io_c)
{
	size_t	i;

	if (!ios)
		return ;
	i = 0;
	while (i < io_c)
		free(ios[i++].arg);
	free(ios);
}

void	free_exec(t_exec *e)
{
	size_t	i;

	if (!e)
		return ;
	i = 0;
	while (i < e->cmd_c)
	{
		free_argv(e->cmds[i].argv);
		free_ios(e->cmds[i].ios, e->cmds[i].io_c);
		free(e->cmds[i].resolved_path);
		i++;
	}
	free(e->cmds);
	free(e);
}

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
    print_tokenized_line(tokenized_line);
	exec = fill_exec(tokenized_line);
    if (!exec)
	{
		free_lexed_line(lexed_line);
		return (free_tokenized_line(tokenized_line));
	}
    // -- LLAMAR AL EXEC DE NICO
	//print_exec(exec); //auxiliar de imprimir
	free_exec(exec);
    free_lexed_line(lexed_line);
    free_tokenized_line(tokenized_line);
}