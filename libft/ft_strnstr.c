/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adumaine <adumaine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 16:04:59 by adumaine          #+#    #+#             */
/*   Updated: 2025/04/08 10:45:00 by adumaine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *s1, const char *s2, size_t len)
{
	size_t	count;
	size_t	i;

	count = 0;
	if (s2[0] == '\0')
		return ((char *)s1);
	while (s1[count] != '\0' && len > count)
	{
		i = 0;
		while (s1[count + i] == s2[i] && s2[i] != '\0' && (count + i) < len)
		{
			i++;
		}
		if (s2[i] == '\0')
			return ((char *)&s1[count]);
		count++;
	}
	return (NULL);
}

/*int main(){
	char str [] = "Vient la desuite";
	printf("trouvé : %s", ft_strnstr(str, "la", 7));
}*/
