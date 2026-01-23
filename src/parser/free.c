/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irrevuel <irrevuel@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 00:14:44 by irrevuel          #+#    #+#             */
/*   Updated: 2026/01/24 00:22:32 by irrevuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"
#include "parser.h"

void	free_lexed_line(t_lexer *lexed_line)
{
	t_lexer		*temp_lexer;
	t_lexpart	*temp_part;

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

void	free_tokenized_line(t_token *tokenized_line)
{
	t_token		*temp_token;
	t_tokenpart	*temp_part;

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
	{
		if (ios[i].expand == -1)
			unlink(ios[i].arg);
		free(ios[i].arg);
		i++;
	}
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
