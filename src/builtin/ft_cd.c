/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antho <antho@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 11:49:37 by antho             #+#    #+#             */
/*   Updated: 2026/02/23 23:16:01 by antho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	cd_update_pwd(t_env *env, char *oldpwd)
{
	char	newpwd[1024];

	if (oldpwd && oldpwd[0])
		update_env(env, "OLDPWD", oldpwd);
	if (!getcwd(newpwd, sizeof(newpwd)))
	{
		perror("getcwd");
		return (1);
	}
	update_env(env, "PWD", newpwd);
	return (0);
}

static char	*get_target_path(t_shell *shell, t_cmd *cmd)
{
	char	*path;

	if (!cmd->args[1])
	{
		path = get_env_value(shell->env, "HOME");
		if (!path)
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		return (path);
	}
	return (cmd->args[1]);
}

static void	set_current_dir(t_shell *shell, char *current_dir)
{
	char	*env_pwd;

	if (!getcwd(current_dir, 1024))
	{
		env_pwd = get_env_value(shell->env, "PWD");
		if (env_pwd)
			ft_strlcpy(current_dir, env_pwd, 1024);
		else
			current_dir[0] = '\0';
	}
}

int	ft_cd(t_shell *shell, t_cmd *cmd)
{
	char	*path;
	char	current_dir[1024];

	set_current_dir(shell, current_dir);
	if (cmd->args && cmd->args[1] && cmd->args[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (1);
	}
	path = get_target_path(shell, cmd);
	if (!path)
		return (1);
	if (chdir(path) == -1)
	{
		perror("minishell: cd");
		return (1);
	}
	return (cd_update_pwd(shell->env, current_dir));
}
