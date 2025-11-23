/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_resolver.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbuquet- <nbuquet-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 16:56:12 by nbuquet-          #+#    #+#             */
/*   Updated: 2025/11/23 19:36:55 by nbuquet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*resolve_path(const char *cmd, char **envp)
{
	if (!cmd || !envp)
		return (NULL);
	if (is_absolute(cmd))
		return (resolve_absolute(cmd));
	return (resolve_cmd(cmd, envp));
}
