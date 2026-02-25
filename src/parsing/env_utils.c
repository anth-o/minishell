/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adumaine <adumaine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 18:51:28 by adumaine          #+#    #+#             */
/*   Updated: 2026/02/25 18:25:47 by adumaine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_tab(char **tab)
{
	int	i;

	if (!tab)
		return ;
	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

int	env_size(t_env *env)
{
	int	i;

	i = 0;
	while (env)
	{
		i++;
		env = env->next;
	}
	return (i);
}

static char	*join_key_value(char *key, char *value)
{
	char	*str;
	int		len;
	int		i;
	int		j;

	if (!key || !value)
		return (NULL);
	len = ft_strlen(key) + ft_strlen(value) + 2;
	str = malloc(sizeof(char) * len);
	if (!str)
		return (NULL);
	i = 0;
	j = 0;
	while (key[i])
		str[j++] = key[i++];
	str[j++] = '=';
	i = 0;
	while (value[i])
		str[j++] = value[i++];
	str[j] = '\0';
	return (str);
}

char	**env_list_to_tab(t_env *env)
{
	char	**tab;
	int		size;
	int		i;

	size = env_size(env);
	tab = malloc(sizeof(char *) * (size + 1));
	if (!tab)
		return (NULL);
	i = 0;
	while (env)
	{
		if (env->value)
		{
			tab[i] = join_key_value(env->key, env->value);
			if (!tab[i])
			{
				free_tab(tab);
				return (NULL);
			}
			i++;
		}
		env = env->next;
	}
	tab[i] = NULL;
	return (tab);
}
