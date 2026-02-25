/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_vars_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adumaine <adumaine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 22:30:12 by adumaine          #+#    #+#             */
/*   Updated: 2026/02/25 18:25:33 by adumaine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	get_len(int n)
{
	int	len;

	len = 0;
	if (n <= 0)
		len++;
	while (n)
	{
		n /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char	*str;
	int		len;
	long	nbr;

	nbr = n;
	len = get_len(n);
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	str[len--] = '\0';
	if (nbr == 0)
		str[0] = '0';
	if (nbr < 0)
	{
		str[0] = '-';
		nbr = -nbr;
	}
	while (nbr > 0)
	{
		str[len--] = (nbr % 10) + '0';
		nbr /= 10;
	}
	return (str);
}

int	var_name_len(const char *s)
{
	int	i;

	i = 0;
	if (s[0] == '?' || (s[0] >= '0' && s[0] <= '9'))
		return (1);
	while (s[i] && (ft_isalnum(s[i]) || s[i] == '_'))
		i++;
	return (i);
}

char	*append_char(char *str, char c)
{
	char	*new;
	int		len;

	len = 0;
	if (str)
		len = ft_strlen(str);
	new = ft_calloc(len + 2, sizeof(char));
	if (!new)
		return (NULL);
	if (str)
	{
		ft_strlcat(new, str, len + 2);
		free(str);
	}
	new[len] = c;
	return (new);
}

char	*append_str(char *dest, char *src)
{
	char	*new;
	int		ldest;
	int		lsrc;

	ldest = 0;
	lsrc = 0;
	if (dest)
		ldest = ft_strlen(dest);
	if (src)
		lsrc = ft_strlen(src);
	new = ft_calloc(ldest + lsrc + 1, sizeof(char));
	if (!new)
		return (NULL);
	if (dest)
	{
		ft_strlcat(new, dest, ldest + lsrc + 1);
		free(dest);
	}
	if (src)
		ft_strlcat(new, src, ldest + lsrc + 1);
	return (new);
}
