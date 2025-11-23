/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbuquet- <nbuquet-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 19:52:19 by nico              #+#    #+#             */
/*   Updated: 2025/11/23 18:48:39 by nbuquet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	dstlen;

	dstlen = ft_strlen(dst);
	if (size == 0 || dstlen >= size)
		return (ft_strlen(src) + size);
	i = dstlen;
	while (i < (size - 1) && src[i - dstlen])
	{
		dst[i] = src[i - dstlen];
		i++;
	}
	dst[i] = '\0';
	return (dstlen + ft_strlen(src));
}
