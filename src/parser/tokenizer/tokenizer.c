/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irrevuel <irrevuel@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 22:46:17 by irrevuel          #+#    #+#             */
/*   Updated: 2026/01/23 23:51:10 by irrevuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

t_tokentype	set_type(t_lexpart *part)
{
	if (part->type == IN_SQUOTE || part->type == IN_DQUOTE)
		return (WORD);
	if (ft_strncmp(part->value, "|", 1) == 0)
		return (PIPE);
	if (ft_strncmp(part->value, "<<", 2) == 0)
		return (HEREDOC);
	if (ft_strncmp(part->value, ">>", 2) == 0)
		return (APPEND);
	if (ft_strncmp(part->value, "<", 1) == 0)
		return (REDIR_IN);
	if (ft_strncmp(part->value, ">", 1) == 0)
		return (REDIR_OUT);
	return (WORD);
}

t_token	*tokenizer(t_lexer *lexed_line)
{
	t_token		*tokens;
	t_token		*aux;
	t_lexpart	*current_part;

	tokens = NULL;
	add_token(&tokens);
	aux = tokens;
	while (lexed_line)
	{
		current_part = lexed_line->parts;
		while (current_part)
		{
			add_token_part(aux, set_type(current_part), current_part->value,
				current_part->type);
			current_part = current_part->next;
		}
		lexed_line = lexed_line->next;
		if (lexed_line)
		{
			add_token(&tokens);
			aux = aux->next;
		}
	}
	return (tokens);
}
