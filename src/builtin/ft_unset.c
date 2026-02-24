/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antho <antho@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 12:10:00 by antho             #+#    #+#             */
/*   Updated: 2026/02/23 21:56:07 by antho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_valid_name(const char *name)
{
	int	i;

	if (!name || (!ft_isalpha((int)name[0]) && name[0] != '_'))
		return (0);
	i = 1;
	while (name[i])
	{
		if (!ft_isalnum((int)name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	remove_env_var(t_shell *shell, char *key)
{
	t_env	*prev;
	t_env	*cur;

	prev = NULL;
	cur = shell->env;
	while (cur)
	{
		if (ft_strcmp(cur->key, key) == 0)
		{
			if (prev)
				prev->next = cur->next;
			else
				shell->env = cur->next;
			free(cur->key);
			if (cur->value)
				free(cur->value);
			free(cur);
			return ;
		}
		prev = cur;
		cur = cur->next;
	}
}

int	ft_unset(t_shell *shell, t_cmd *cmd)
{
	int	i;

	if (!cmd || !cmd->args)
		return (0);
	i = 1;
	while (cmd->args[i])
	{
		if (!is_valid_name(cmd->args[i]))
		{
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(cmd->args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
		}
		else
		{
			remove_env_var(shell, cmd->args[i]);
		}
		i++;
	}
	return (0);
}
