/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antho <antho@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 10:36:20 by antho             #+#    #+#             */
/*   Updated: 2025/04/14 11:19:06 by antho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_countwords(char const *s, char c)
{
	size_t	count;
	size_t	i;

	i = 0;
	if (!s[i])
		return (0);
	count = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i])
			count++;
		while (s[i] && s[i] != c)
			i++;
	}
	return (count);
}

void	free_all(char **tab, size_t j)
{
	while (j--)
		free(tab[j]);
	free(tab);
}

char	*wordcpy(const char *s, char c)
{
	char	*word;
	int		len;

	len = 0;
	while (s[len] && s[len] != c)
		len++;
	word = malloc(len + 1);
	if (!word)
		return (NULL);
	ft_strlcpy(word, s, len + 1);
	return (word);
}

char	**ft_split(char const *s, char c)
{
	char	**result;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	result = malloc((ft_countwords(s, c) + 1) * sizeof(char *));
	if (!s || !result)
		return (NULL);
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i])
		{
			result[j] = wordcpy(&s[i], c);
			if (!result[j])
				return (free_all(result, j), NULL);
			i += ft_strlen(result[j]);
			j++;
		}
	}
	result[j] = NULL;
	return (result);
}

/*int	main(void)
{
	char **result;
	size_t i;

	i = 0;
	result = ft_split("Coucouc comment ça va?", ' ');
	while (result[i])
	{
		printf("%s\n", result[i]);
		i++;
	}
	return (0);
}*/
