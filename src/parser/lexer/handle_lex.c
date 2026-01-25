/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_lex.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irrevuel <irrevuel@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 22:34:21 by irrevuel          #+#    #+#             */
/*   Updated: 2026/01/24 13:56:21 by irrevuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

int	is_special(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

static int	handle_quotes(char *line, int i, t_lexer *tok, t_lexer_type mode)
{
	int		start;
	char	quote;

	if (mode == IN_SQUOTE)
		quote = '\'';
	else
		quote = '"';
	start = ++i;
	while (line[i] && line[i] != quote)
		i++;
	if (!line[i])
	{
		ft_putstr_fd("Error: open quote\n", 2);
		return (-1);
	}
	add_part(tok, mode, ft_substr(line, start, i - start));
	if (line[i] == quote)
		i++;
	return (i);
}

static int	handle_special_or_text(char *line, int i, t_lexer *tok)
{
	int	start;

	start = i;
	if (line[i] && is_special(line[i]))
	{
		if ((line[i] == '<' && line[i + 1] == '<') || (line[i] == '>' && line[i
					+ 1] == '>'))
		{
			add_part(tok, IN_DEFAULT, ft_substr(line, i, 2));
			return (i + 2);
		}
		add_part(tok, IN_DEFAULT, ft_substr(line, i, 1));
		return (i + 1);
	}
	while (line[i] && !isspace((unsigned char)line[i]) && line[i] != '\''
		&& line[i] != '"' && !is_special(line[i]))
		i++;
	if (i > start)
		add_part(tok, IN_DEFAULT, ft_substr(line, start, i - start));
	return (i);
}

int	handle_mode(char *line, int i, t_lexer *tok, t_lexer_type mode)
{
	if (mode == IN_SQUOTE || mode == IN_DQUOTE)
		return (handle_quotes(line, i, tok, mode));
	return (handle_special_or_text(line, i, tok));
}
