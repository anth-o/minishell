/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antho <antho@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 22:02:08 by antho             #+#    #+#             */
/*   Updated: 2026/02/23 22:05:10 by antho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	count_cmds(t_cmd *cmd)
{
	int	count;

	count = 0;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}

void	close_and_free_pipes(int **pipes, int count)
{
	int	j;

	if (!pipes)
		return ;
	j = 0;
	while (j < count - 1)
	{
		close(pipes[j][0]);
		close(pipes[j][1]);
		free(pipes[j]);
		j++;
	}
	free(pipes);
}

void	setup_child_pipes(int i, int count, int **pipes)
{
	int	j;

	if (count > 1)
	{
		if (i > 0)
			if (dup2(pipes[i - 1][0], STDIN_FILENO) == -1)
				exit(1);
		if (i < count - 1)
			if (dup2(pipes[i][1], STDOUT_FILENO) == -1)
				exit(1);
		j = 0;
		while (j < count - 1)
		{
			close(pipes[j][0]);
			close(pipes[j][1]);
			j++;
		}
	}
}

void	wait_children(pid_t *pids, int count, t_shell *shell)
{
	int	i;
	int	status;
	int	sig;

	i = 0;
	while (i < count)
	{
		waitpid(pids[i], &status, 0);
		if (i == count - 1)
		{
			if (WIFEXITED(status))
				shell->exit_code = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			{
				sig = WTERMSIG(status);
				if (sig == SIGQUIT)
					ft_putstr_fd("Quit (core dumped)\n", 2);
				else if (sig == SIGINT)
					write(1, "\n", 1);
				shell->exit_code = 128 + sig;
			}
		}
		i++;
	}
	free(pids);
}

int	**allocate_pipes(int count)
{
	int	**pipes;
	int	i;

	if (count <= 1)
		return (NULL);
	pipes = malloc(sizeof(int *) * (count - 1));
	if (!pipes)
		return (NULL);
	i = 0;
	while (i < count - 1)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i] || pipe(pipes[i]) == -1)
			return (NULL);
		i++;
	}
	return (pipes);
}
