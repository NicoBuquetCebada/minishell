/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_duper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbuquet- <nbuquet-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 22:32:37 by nbuquet-          #+#    #+#             */
/*   Updated: 2026/01/24 22:41:19 by nbuquet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"

static void	free_envp_from(char **copy, size_t i);

char	**dup_envp(char **envp)
{
	size_t	i;
	size_t	count;
	char	**copy;

	if (!envp)
		return (NULL);
	count = 0;
	while (envp[count])
		count++;
	copy = malloc(sizeof(char *) * (count + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (i < count)
	{
		copy[i] = ft_strdup(envp[i]);
		if (!copy[i])
			return (free_envp_from(copy, i), NULL);
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

static void	free_envp_from(char **copy, size_t i)
{
	while (i > 0)
		free(copy[--i]);
	free(copy);
}

void	free_envp(char **envp)
{
	size_t	i;

	if (!envp)
		return ;
	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}
