/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 17:35:40 by nico              #+#    #+#             */
/*   Updated: 2024/09/29 18:07:20 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list			*t;

	if (!lst[0])
		lst[0] = new;
	else
	{
		t = *lst;
		while (t -> next != NULL)
			t = t -> next;
		t -> next = new;
	}
}
