/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_resolver_cmd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbuquet- <nbuquet-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 19:31:20 by nbuquet-          #+#    #+#             */
/*   Updated: 2025/11/23 19:51:05 by nbuquet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_pathenv(char **envp)
{
	size_t	i;

	if (!envp)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (ft_strdup(envp[i] + 5));
		i++;
	}
	return (NULL);
}

static char	*get_fullcmdpath(const char *dir, const char *cmd)
{
	size_t	dir_len;
	size_t	cmd_len;
	char	*cmdpath;
	size_t	i;

	if (!dir || !cmd)
		return (NULL);
	dir_len = ft_strlen(dir);
	cmd_len = ft_strlen(cmd);
	cmdpath = (char *)malloc((dir_len + cmd_len + 2) * sizeof(char));
	if (!cmdpath)
		return (NULL);
	i = -1;
	while (++i < dir_len)
		cmdpath[i] = dir[i];
	cmdpath[i] = '/';
	i++;
	while (*cmd)
	{
		cmdpath[i] = *cmd++;
		i++;
	}
	cmdpath[i] = '\0';
	return (cmdpath);
}

char	*resolve_cmd(const char *cmd, char **envp)
{
	char	*pathenv;
	char	*cmdpath;
	char	*next;
	char	*pathenv_clean;

	pathenv_clean = get_pathenv(envp);
	pathenv = pathenv_clean;
	if (!pathenv || !*pathenv)
		return (NULL);
	while (1)
	{
		next = ft_strchr(pathenv, ':');
		if (next)
			*next = '\0';
		cmdpath = get_fullcmdpath(pathenv, cmd);
		if (!cmdpath)
			break ;
		if (access(cmdpath, X_OK) == 0)
			return (free(pathenv_clean), cmdpath);
		free(cmdpath);
		if (!next)
			break ;	
		pathenv = next + 1;
	}
	return (free(pathenv_clean), NULL);
}
