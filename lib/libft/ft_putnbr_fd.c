/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 15:50:15 by nico              #+#    #+#             */
/*   Updated: 2024/10/04 17:10:47 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_recursivewrite(int n, int fd)
{
	char	r;

	if (n == 0)
		return ;
	ft_recursivewrite(n / 10, fd);
	r = (n % 10) + '0';
	write(fd, &r, 1);
}

void	ft_putnbr_fd(int n, int fd)
{
	if (n == -2147483648)
		write(fd, "-2147483648", 11);
	else if (n < 0)
	{
		n *= -1;
		write(fd, "-", 1);
		ft_recursivewrite(n, fd);
	}
	else if (n == 0)
		write(fd, "0", 1);
	else
		ft_recursivewrite(n, fd);
}
