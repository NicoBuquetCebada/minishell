/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_resolver.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbuquet- <nbuquet-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 16:56:12 by nbuquet-          #+#    #+#             */
/*   Updated: 2026/01/19 19:41:50 by nbuquet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "executor.h"

char	*resolve_path(char *cmd, char **envp)
{
	if (!cmd || !envp)
		return (NULL);
	if (is_absolute(cmd))
		return (resolve_absolute(cmd));
	return (resolve_cmd(cmd, envp));
}
