/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 23:39:07 by nico              #+#    #+#             */
/*   Updated: 2024/10/04 17:11:13 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	**ft_freesplit(char **dst)
{
	unsigned int	i;

	i = 0;
	while (dst[i])
	{
		free (dst[i]);
		i++;
	}
	free (dst);
	return (NULL);
}

static unsigned int	ft_wrdcnt(const char *s, char c)
{
	unsigned int	cntr;
	unsigned int	i;

	i = 0;
	cntr = 0;
	while (s[i])
	{
		if (s[i] == c)
			i++;
		else
		{
			cntr++;
			while (s[i] != c && s[i])
				i++;
		}
	}
	return (cntr);
}

static char	**ft_splitcpy(char **dst, const char *s,
	unsigned int start, unsigned int end)
{
	unsigned int	len;
	unsigned int	i;

	len = end - start;
	i = 0;
	*dst = (char *)malloc((len + 1) * sizeof(char));
	if (!(*dst))
		return (NULL);
	while (i < len)
	{
		(*dst)[i] = s[start + i];
		i++;
	}
	(*dst)[i] = '\0';
	return (dst);
}

char	**ft_split(char const *s, char c)
{
	char			**dst;
	unsigned int	i;
	unsigned int	start;
	unsigned int	index;

	dst = (char **)malloc((ft_wrdcnt(s, c) + 1) * (sizeof(char *)));
	if (!dst)
		return (NULL);
	i = 0;
	index = 0;
	while (s[i])
	{
		if (s[i] == c)
			i++;
		else
		{
			start = i;
			while (s[i] != c && s[i])
				i++;
			if (!ft_splitcpy(&dst[index++], s, start, i))
				return (ft_freesplit(dst));
		}
	}
	dst[index] = NULL;
	return (dst);
}
