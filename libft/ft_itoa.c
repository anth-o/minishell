/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antho <antho@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 22:17:00 by antho             #+#    #+#             */
/*   Updated: 2025/04/13 18:28:03 by antho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	result_len(int n)
{
	size_t	len;

	len = 0;
	if (n <= 0)
		len = 1;
	while (n)
	{
		len++;
		n /= 10;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	size_t	len;
	char	*result;

	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	len = result_len(n);
	result = malloc((len + 1) * sizeof(char));
	if (!result)
		return (NULL);
	result[len] = '\0';
	if (n < 0)
	{
		result[0] = '-';
		n = -n;
	}
	if (n == 0)
		result[0] = '0';
	while (n)
	{
		len--;
		result[len] = (n % 10) + '0';
		n /= 10;
	}
	return (result);
}
