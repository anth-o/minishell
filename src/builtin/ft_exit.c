/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antho <antho@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 01:16:57 by antho             #+#    #+#             */
/*   Updated: 2026/02/15 18:58:44 by antho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_shell(t_shell *shell)
{
	if (shell->env)
		free_env(shell->env);
	rl_clear_history();
}

int	ft_exit(t_shell *shell, t_cmd *cmd)
{
	printf("exit\n");
	free_shell(shell);
	if (cmd)
		free_cmds(cmd);
	exit(0);
	return (0);
}
