/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 21:16:02 by nico              #+#    #+#             */
/*   Updated: 2024/09/27 17:28:35 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*ptrdest;
	unsigned char	*ptrsrc;

	ptrdest = (unsigned char *)dest;
	ptrsrc = (unsigned char *)src;
	if (dest < src)
	{
		i = -1;
		while (++i < n)
			ptrdest[i] = ptrsrc[i];
	}
	if (dest > src)
	{
		i = n - 1;
		while (n > 0)
		{
			ptrdest[i] = ptrsrc[i];
			i--;
			n--;
		}
	}
	return (dest);
}
