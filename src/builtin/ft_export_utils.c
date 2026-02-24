/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antho <antho@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 21:30:22 by antho             #+#    #+#             */
/*   Updated: 2026/02/23 21:30:45 by antho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_valid_key(const char *key)
{
	int	i;

	if (!key || (!ft_isalpha((int)key[0]) && key[0] != '_'))
		return (0);
	i = 1;
	while (key[i] && key[i] != '=')
	{
		if (!ft_isalnum((int)key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	sort_env_tab(t_env **tab, int size)
{
	int		i;
	int		j;
	t_env	*tmp;

	i = 0;
	while (i < size - 1)
	{
		j = i + 1;
		while (j < size)
		{
			if (ft_strcmp(tab[i]->key, tab[j]->key) > 0)
			{
				tmp = tab[i];
				tab[i] = tab[j];
				tab[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

static void	print_sorted_tab(t_env **tab, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		if (tab[i]->value)
			printf("declare -x %s=\"%s\"\n", tab[i]->key, tab[i]->value);
		else
			printf("declare -x %s\n", tab[i]->key);
		i++;
	}
}

void	print_export(t_env *env)
{
	t_env	**tab;
	int		size;
	int		i;
	t_env	*tmp;

	size = 0;
	tmp = env;
	while (tmp && ++size)
		tmp = tmp->next;
	tab = malloc(sizeof(t_env *) * size);
	if (!tab)
		return ;
	i = 0;
	tmp = env;
	while (tmp)
	{
		tab[i++] = tmp;
		tmp = tmp->next;
	}
	sort_env_tab(tab, size);
	print_sorted_tab(tab, size);
	free(tab);
}
