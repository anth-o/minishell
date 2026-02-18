/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ko-mahon <ko-mahon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 01:16:57 by antho             #+#    #+#             */
/*   Updated: 2026/02/18 20:12:09 by ko-mahon         ###   ########.fr       */
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
	long val;
	char *endptr;

	printf("exit\n");
	if (!cmd || !cmd->args || !cmd->args[1])
	{
		free_shell(shell);
		if (cmd)
			free_cmds(cmd);
		exit(0);
	}
	/* Too many arguments: do not exit, return error */
	if (cmd->args[1] && cmd->args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	/* Single arg: attempt to parse as number */
	errno = 0;
	val = strtol(cmd->args[1], &endptr, 10);
	if (*endptr != '\0' || errno == ERANGE)
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(cmd->args[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		free_shell(shell);
		if (cmd)
			free_cmds(cmd);
		exit(2);
	}
	free_shell(shell);
	if (cmd)
		free_cmds(cmd);
	exit((int)(val & 0xFF));
	return (0);
}
