/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ko-mahon <ko-mahon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 12:10:00 by antho             #+#    #+#             */
/*   Updated: 2026/02/18 17:54:15 by ko-mahon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_valid_name(const char *name)
{
    int i;

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

int	ft_unset(t_shell *shell, t_cmd *cmd)
{
    int i;
    t_env *prev;
    t_env *cur;

    if (!cmd || !cmd->args)
        return (0);
    i = 1;
    while (cmd->args[i])
    {
        if (!is_valid_name(cmd->args[i]))
        {
            fprintf(stderr, "minishell: unset: `%s': not a valid identifier\n", cmd->args[i]);
            i++;
            continue ;
        }
        prev = NULL;
        cur = shell->env;
        while (cur)
        {
            if (ft_strcmp(cur->key, cmd->args[i]) == 0)
            {
                if (prev)
                    prev->next = cur->next;
                else
                    shell->env = cur->next;
                if (cur->key)
                    free(cur->key);
                if (cur->value)
                    free(cur->value);
                free(cur);
                break ;
            }
            prev = cur;
            cur = cur->next;
        }
        i++;
    }
    return (0);
}
