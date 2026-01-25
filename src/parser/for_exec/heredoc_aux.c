/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_aux.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irrevuel <irrevuel@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 13:31:16 by irrevuel          #+#    #+#             */
/*   Updated: 2026/01/24 13:32:26 by irrevuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"
#include "parser.h"

int	is_delim(char *line, char *delim)
{
	return (line && delim && ft_strcmp(line, delim) == 0);
}

int	write_ln(int fd, char *s)
{
	if (!s)
		return (1);
	if (write(fd, s, ft_strlen(s)) < 0)
		return (0);
	if (write(fd, "\n", 1) < 0)
		return (0);
	return (1);
}
