/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adumaine <adumaine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 12:05:00 by adumaine          #+#    #+#             */
/*   Updated: 2026/02/25 18:26:57 by adumaine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_env	*find_env(t_env *env, const char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

static void	add_env_node(t_env **env, char *key, char *val)
{
	t_env	*new;
	t_env	*tmp;

	new = malloc(sizeof(t_env));
	if (!new)
		return ;
	new->key = ft_strdup(key);
	new->value = NULL;
	if (val)
		new->value = ft_strdup(val);
	new->next = NULL;
	if (!*env)
	{
		*env = new;
		return ;
	}
	tmp = *env;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

static void	handle_export_arg(t_shell *shell, char *arg)
{
	char	*eq;
	char	*key;
	char	*val;
	t_env	*found;

	eq = ft_strchr(arg, '=');
	if (eq)
	{
		key = ft_substr(arg, 0, eq - arg);
		val = eq + 1;
	}
	else
	{
		key = ft_strdup(arg);
		val = NULL;
	}
	found = find_env(shell->env, key);
	if (found && val)
	{
		free(found->value);
		found->value = ft_strdup(val);
	}
	else if (!found)
		add_env_node(&shell->env, key, val);
	free(key);
}

int	ft_export(t_shell *shell, t_cmd *cmd)
{
	int	i;
	int	ret;

	if (!cmd || !cmd->args)
		return (0);
	if (!cmd->args[1])
	{
		print_export(shell->env);
		return (0);
	}
	ret = 0;
	i = 0;
	while (cmd->args[++i])
	{
		if (!is_valid_key(cmd->args[i]))
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(cmd->args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			ret = 1;
		}
		else
			handle_export_arg(shell, cmd->args[i]);
	}
	return (ret);
}
