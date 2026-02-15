/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antho <antho@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 14:30:46 by antho             #+#    #+#             */
/*   Updated: 2026/02/14 15:14:10 by antho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_env	*create_env_node(char *str)
{
	t_env	*node;
	char	*pos_eq;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	pos_eq = strchr(str, '=');
	if (pos_eq)
	{
		node->key = strndup(str, pos_eq - str);
		node->value = strdup(pos_eq + 1);
	}
	else
	{
		node->key = strdup(str);
		node->value = NULL;
	}
	node->next = NULL;
	return (node);
}

static void	env_add_back(t_env **head, t_env *new)
{
	t_env	*tmp;

	if (!*head)
	{
		*head = new;
		return ;
	}
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		if (env->key)
			free(env->key);
		if (env->value)
			free(env->value);
		free(env);
		env = tmp;
	}
}

t_env	*init_env(char **envp)
{
	t_env	*head;
	t_env	*new;
	int		i;

	head = NULL;
	i = 0;
	while (envp[i])
	{
		new = create_env_node(envp[i]);
		if (!new)
		{
			free_env(head);
			return (NULL);
		}
		env_add_back(&head, new);
		i++;
	}
	return (head);
}
