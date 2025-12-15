/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_resolver_absolute.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbuquet- <nbuquet-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 19:31:17 by nbuquet-          #+#    #+#             */
/*   Updated: 2025/12/08 21:24:04 by nbuquet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*resolve_absolute(char *cmd)
{
	struct stat	st;

	if (access(cmd, F_OK) != 0)
		bin_error(cmd, ENOENT);
	if (stat(cmd, &st) == 0 && S_ISDIR(st.st_mode))
		bin_error(cmd, EISDIR);
	if (access(cmd, X_OK) != 0)
		bin_error(cmd, EACCES);
	return (ft_strdup(cmd));
}

int	is_absolute(char *cmd)
{
	size_t	i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '/')
			return (1);
		i++;
	}
	return (0);
}
