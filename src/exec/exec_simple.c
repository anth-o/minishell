/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_simple.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antho <antho@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 19:20:58 by antho             #+#    #+#             */
/*   Updated: 2026/02/15 18:30:39 by antho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <sys/wait.h> // Pour waitpid

void	handle_cmd_execution(t_cmd *cmd, t_shell *shell)
{
	char	*path;
	char	**env_tab;

	// 1. On transforme l'environnement en tableau pour execve
	env_tab = env_list_to_tab(shell->env);
	// 2. On cherche le chemin absolu de la commande
	path = find_path(cmd->args[0], shell->env);
	if (!path)
	{
		// Cas où la commande n'existe pas
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		free_tab(env_tab);
		exit(127); // 127 est le code standard pour "command not found"
	}
	// 3. On lance la commande !
	// Si execve réussit, ce processus est REMPLACÉ par ls.
	// Si on est encore là après cette ligne, c'est que ça a raté.
	if (execve(path, cmd->args, env_tab) == -1)
	{
		perror("execve");
		free(path);
		free_tab(env_tab);
		exit(1);
	}
}

void	exec_simple_cmd(t_cmd *cmd, t_shell *shell)
{
	pid_t	pid;
	int		status;

	if (!cmd->args || !cmd->args[0])
		return ;
	// 1. EST-CE UN BUILTIN ?
	// Si oui, on l'exécute directement dans le PARENT
	if (is_builtin(cmd->args[0]))
	{
		// Note : Les redirections pour les builtins (ex: pwd > file) sont complexes
		// sans fork. Pour l'instant, on lance le builtin simplement.
		shell->exit_code = exec_builtin(cmd, shell);
		return ;
	}
	// 2. SINON, C'EST UN PROGRAMME EXTERNE -> FORK
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return ;
	}
	if (pid == 0)
	{
		if (handle_redirections(cmd))
			exit(1);
		handle_cmd_execution(cmd, shell);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			shell->exit_code = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			shell->exit_code = 128 + WTERMSIG(status);
	}
}
