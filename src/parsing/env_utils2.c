/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antho <antho@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 22:22:49 by antho             #+#    #+#             */
/*   Updated: 2026/02/24 00:36:36 by antho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	update_env(t_env *env, const char *key, const char *value)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
		{
			free(tmp->value);
			tmp->value = ft_strdup(value);
			return ;
		}
		tmp = tmp->next;
	}
}

char	*get_env_value(t_env *env, char *key)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}

void	increment_shlvl(t_env *env)
{
	char	*val;
	int		lvl;
	char	*new_val;

	val = get_env_value(env, "SHLVL");
	if (val)
		lvl = ft_atoi(val) + 1;
	else
		lvl = 1;
	new_val = ft_itoa(lvl);
	if (new_val)
	{
		update_env(env, "SHLVL", new_val);
		free(new_val);
	}
}
