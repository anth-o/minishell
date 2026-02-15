/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antho <antho@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 19:21:03 by antho             #+#    #+#             */
/*   Updated: 2025/04/12 12:28:46 by antho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(const char *s1, const char *s2)
{
	size_t	len_s1;
	size_t	len_s2;
	char	*result;

	if (!s1 || !s2)
		return (NULL);
	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	result = malloc(len_s1 + len_s2 + 1);
	if (!result)
		return (NULL);
	result[0] = '\0';
	ft_strlcat(result, s1, len_s1 + len_s2 + 1);
	ft_strlcat(result, s2, len_s1 + len_s2 + 1);
	return (result);
}
