/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adumaine <adumaine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 00:57:53 by adumaine          #+#    #+#             */
/*   Updated: 2026/02/25 18:26:52 by adumaine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_pwd(t_shell *shell)
{
	char	cwd[1024];
	char	*env_pwd;

	if (getcwd(cwd, sizeof(cwd)))
	{
		printf("%s\n", cwd);
		return (0);
	}
	env_pwd = get_env_value(shell->env, "PWD");
	if (env_pwd)
	{
		printf("%s\n", env_pwd);
		return (0);
	}
	perror("minishell: pwd");
	return (1);
}
