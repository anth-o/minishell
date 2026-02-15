/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adumaine <adumaine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 15:12:06 by adumaine          #+#    #+#             */
/*   Updated: 2025/04/08 10:45:02 by adumaine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int	i;

	i = ft_strlen(s) - 1;
	if ((char)c == '\0')
		return ((char *)&s[i + 1]);
	while (i >= 0)
	{
		if (s[i] == (char)c)
			return ((char *)&s[i]);
		i--;
	}
	return (NULL);
}

/*#include <stdio.h>
#include <string.h>

int	main(void) {
	const char *str = "Bonjour tout le monde";
	char *result = ft_strrchr(str, '\0');

	if (result != NULL)
		printf("%s\n", result);
	else
		printf("Caractère non trouvé.\n");

	return (0);
}*/