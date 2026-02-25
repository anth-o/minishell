/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adumaine <adumaine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 18:58:44 by adumaine          #+#    #+#             */
/*   Updated: 2026/02/25 18:26:24 by adumaine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*get_env_val(t_env *env, char *key)
{
	while (env)
	{
		if (env->key && ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

static char	*search_in_paths(char **paths, char *cmd)
{
	char	*path_part;
	char	*full_path;
	int		i;

	i = -1;
	while (paths[++i])
	{
		path_part = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(path_part, cmd);
		free(path_part);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
	}
	return (NULL);
}

char	*find_path(char *cmd, t_env *env)
{
	char	**paths;
	char	*result;
	char	*path_env;

	if (cmd && ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	path_env = get_env_val(env, "PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	result = search_in_paths(paths, cmd);
	free_tab(paths);
	return (result);
}
