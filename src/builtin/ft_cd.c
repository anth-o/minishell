/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antho <antho@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 11:49:37 by antho             #+#    #+#             */
/*   Updated: 2026/02/15 18:44:21 by antho            ###   ########.fr       */
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

int	ft_cd(t_shell *shell, t_cmd *cmd)
{
	char	*path;
	char	current_dir[1024];

	if (!getcwd(current_dir, sizeof(current_dir)))
		return (1);
	if (!cmd->args[1])
	{
		path = get_env_value(shell->env, "HOME");
		if (!path)
		{
			perror("cd: HOME not set");
			return (1);
		}
	}
	else
		path = cmd->args[1];
	if (chdir(path) == -1)
	{
		perror("cd");
		return (1);
	}
	return (cd_update_pwd(shell->env, current_dir));
}
