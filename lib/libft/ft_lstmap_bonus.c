/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 18:34:46 by nico              #+#    #+#             */
/*   Updated: 2024/10/05 13:47:53 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*dst;
	t_list	*t;
	void	*c;

	if (!lst || !f || !del)
		return (NULL);
	dst = NULL;
	while (lst)
	{
		c = f(lst -> content);
		t = ft_lstnew(c);
		if (!t)
		{
			del(c);
			ft_lstclear(&dst, del);
			return (NULL);
		}
		ft_lstadd_back(&dst, t);
		lst = lst -> next;
	}
	return (dst);
}
