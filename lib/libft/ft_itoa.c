/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 14:04:35 by nico              #+#    #+#             */
/*   Updated: 2024/10/04 17:09:51 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_intlen(int n)
{
	int	cntr;

	cntr = 0;
	if (n <= 0)
		cntr = 1;
	while (n != 0)
	{
		n /= 10;
		cntr++;
	}
	return (cntr);
}

static void	ft_recursiveitoa(int n, char *cn, int *index)
{
	if (n >= 10)
		ft_recursiveitoa(n / 10, cn, index);
	cn[(*index)++] = (n % 10) + '0';
}

char	*ft_itoa(int n)
{
	int		len;
	char	*cn;
	int		index;

	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	else if (n == 0)
		return (ft_strdup("0"));
	len = ft_intlen(n);
	cn = (char *)malloc((len + 1) * sizeof(char));
	if (!cn)
		return (NULL);
	index = 0;
	if (n < 0)
	{
		cn[index++] = '-';
		n = -n;
	}
	ft_recursiveitoa(n, cn, &index);
	cn[index] = '\0';
	return (cn);
}
