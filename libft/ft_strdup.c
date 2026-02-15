/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adumaine <adumaine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 13:51:36 by adumaine          #+#    #+#             */
/*   Updated: 2025/04/08 10:44:39 by adumaine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_strcpy(char *dst, const char *src)
{
	int	i;

	i = 0;
	while (src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
}

char	*ft_strdup(const char *src)
{
	char	*dst;

	if (!src)
		return (NULL);
	dst = malloc((ft_strlen(src) + 1) * sizeof(char));
	if (!dst)
		return (NULL);
	ft_strcpy(dst, src);
	return (dst);
}
