/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbuquet- <nbuquet-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 21:19:28 by nbuquet-          #+#    #+#             */
/*   Updated: 2025/11/23 20:40:28 by nbuquet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int ac, char *av[], char *ep[])
{
	char	*resolved_path;

	if (ac == 0)
		return (1);
	resolved_path = resolve_path(av[1], ep);
	printf(resolved_path);
	free(resolved_path);
}
