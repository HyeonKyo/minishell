/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyeonkyokim <hyeonkyokim@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 12:35:37 by kyunkim           #+#    #+#             */
/*   Updated: 2021/10/09 22:18:40 by hyeonkyokim      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	my_strcpy(char *dest, const char *src)
{
	int	i;

	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = 0;
}

char	*ft_strdup(const char *src)
{
	int		length;
	char	*ptr;

	if (src == NULL)
		return (NULL);
	length = ft_strlen(src) + 1;
	ptr = (char *)malloc(length);
	if (ptr == NULL)
		return (NULL);
	my_strcpy(ptr, src);
	return (ptr);
}
