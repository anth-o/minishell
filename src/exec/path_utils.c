/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ko-mahon <ko-mahon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 18:58:44 by antho             #+#    #+#             */
/*   Updated: 2026/02/18 20:12:10 by ko-mahon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// Récupère la valeur d'une clé dans l'env (ex: "PATH")
static char	*get_env_val(t_env *env, char *key)
{
	while (env)
	{
		if (env->key && strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

char	*find_path(char *cmd, t_env *env)
{
	char	**paths;
	char	*path_part;
	char	*full_path;
	int		i;

	// 1. Si la commande contient déjà un slash (ex: /bin/ls ou ./minishell)
	if (cmd && strchr(cmd, '/'))
	{
		/* If the path contains a slash, return it if the file exists
		 * even if not executable: execve will set errno and we can map it. */
		if (access(cmd, F_OK) == 0)
			return (strdup(cmd));
		return (NULL);
	}
	// 2. On récupère le PATH
	if (!get_env_val(env, "PATH"))
		return (NULL);
	paths = ft_split(get_env_val(env, "PATH"), ':'); // Besoin de ta libft !
	if (!paths)
		return (NULL);
	// 3. On teste chaque dossier
	i = -1;
	while (paths[++i])
	{
		path_part = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(path_part, cmd);
		free(path_part);
		if (access(full_path, X_OK) == 0) // Trouvé !
		{
			free_tab(paths);
			return (full_path);
		}
		free(full_path);
	}
	free_tab(paths);
	return (NULL);
}
