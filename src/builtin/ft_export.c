/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ko-mahon <ko-mahon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 12:05:00 by antho             #+#    #+#             */
/*   Updated: 2026/02/18 20:12:08 by ko-mahon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_valid_key(const char *key)
{
    int i;

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

static t_env *find_env(t_env *env, const char *key)
{
    t_env *tmp = env;
    while (tmp)
    {
        if (ft_strcmp(tmp->key, key) == 0)
            return (tmp);
        tmp = tmp->next;
    }
    return (NULL);
}

static void	add_env_back(t_env **head, t_env *new)
{
    t_env *tmp;
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

static t_env *create_node_from_kv(const char *key, const char *value)
{
    t_env *node = malloc(sizeof(t_env));
    if (!node)
        return (NULL);
    node->key = ft_strdup(key);
    node->value = value ? ft_strdup(value) : NULL;
    node->next = NULL;
    return (node);
}

int	ft_export(t_shell *shell, t_cmd *cmd)
{
    int i;
    char *arg;
    char *eq;
    int ret;

    if (!cmd || !cmd->args)
        return (0);
    if (!cmd->args[1])
    {
        /* No args: print environment in declare -x format */
        t_env *tmp = shell->env;
        while (tmp)
        {
            if (tmp->value)
                printf("declare -x %s=\"%s\"\n", tmp->key, tmp->value);
            else
                printf("declare -x %s\n", tmp->key);
            tmp = tmp->next;
        }
        return (0);
    }
    i = 1;
    ret = 0;
    while (cmd->args[i])
    {
        arg = cmd->args[i];
        eq = strchr(arg, '=');
        if (!is_valid_key(arg))
        {
            fprintf(stderr, "minishell: export: `%s': not a valid identifier\n", arg);
            ret = 1;
            i++;
            continue ;
        }
        if (eq)
        {
            /* key=val */
            size_t klen = eq - arg;
            char *key = strndup(arg, klen);
            char *val = strdup(eq + 1);
            t_env *found = find_env(shell->env, key);
            if (found)
            {
                free(found->value);
                found->value = val;
                free(key);
            }
            else
            {
                t_env *n = create_node_from_kv(key, val);
                add_env_back(&shell->env, n);
                free(key);
                free(val);
            }
        }
        else
        {
            /* key only: ensure exists with NULL value */
            t_env *found = find_env(shell->env, arg);
            if (!found)
            {
                t_env *n = create_node_from_kv(arg, NULL);
                add_env_back(&shell->env, n);
            }
        }
        i++;
    }
    return (ret);
}
