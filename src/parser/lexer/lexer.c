/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irrevuel <irrevuel@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 22:31:18 by irrevuel          #+#    #+#             */
/*   Updated: 2026/01/23 22:35:30 by irrevuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

static int	process_token(char *line, int i, t_lexer *nodes, t_lexer *aux)
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
		return (-1);
	}
	return (i);
}

static int	skip_spaces(char *line, int i, t_lexer **nodes, t_lexer **aux)
{
	while (line[i] && isspace((unsigned char)line[i]))
		i++;
	if (line[i])
	{
		add_node(nodes);
		*aux = (*aux)->next;
	}
	return (i);
}

t_lexer	*lexer(char *line)
{
	t_lexer	*nodes;
	t_lexer	*aux;
	int		i;

	nodes = NULL;
	if (!line)
		return (NULL);
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
				return (NULL);
		}
	}
	return (nodes);
}
