/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antho <antho@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 10:45:10 by adumaine          #+#    #+#             */
/*   Updated: 2025/04/10 00:59:11 by antho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	start;
	size_t	end;
	char	*result;

	start = 0;
	if (!s1 || !set)
		return (NULL);
	end = ft_strlen(s1);
	while (s1[start] && ft_strchr(set, s1[start]))
		start++;
	while (end > start && ft_strchr(set, s1[end - 1]))
		end--;
	result = ft_substr(s1, start, end - start);
	return (result);
}

/*int	main(void)
{
	printf("%s\n", ft_strtrim("fd  Hello World! fd", "fd"));
	return (0);
}*/