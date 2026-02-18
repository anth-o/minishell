/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ko-mahon <ko-mahon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 12:00:00 by antho             #+#    #+#             */
/*   Updated: 2026/02/18 18:10:52 by ko-mahon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_echo(t_cmd *cmd)
{
    int i;
    int newline;
    if (!cmd || !cmd->args)
        return (0);
    i = 1;
    newline = 1;

    /* Accept -n, -nnn, and multiple -n options (e.g. -n -n) */
    while (cmd->args[i])
    {
        char *s = cmd->args[i];
        int j = 0;
        if (s[j] != '-')
            break ;
        j++;
        if (s[j] == '\0')
            break ;
        while (s[j] && s[j] == 'n')
            j++;
        if (s[j] != '\0')
            break ;
        /* it's an -n (or -nnn...) option */
        newline = 0;
        i++;
    }
    while (cmd->args && cmd->args[i])
    {
        printf("%s", cmd->args[i]);
        if (cmd->args[i + 1])
            printf(" ");
        i++;
    }
    if (newline)
        printf("\n");
    return (0);
}
