/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_simple.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antho <antho@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 19:20:58 by antho             #+#    #+#             */
/*   Updated: 2026/02/24 21:25:35 by antho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_cmd_execution(t_cmd *cmd, t_shell *shell)
{
	char	*path;
	char	**env_tab;
	int		err;

	env_tab = env_list_to_tab(shell->env);
	path = find_path(cmd->args[0], shell->env);
	if (!path)
	{
		ft_putstr_fd(cmd->args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		free_tab(env_tab);
		exit(127);
	}
	if (execve(path, cmd->args, env_tab) == -1)
	{
		err = errno;
		if (err == EACCES || err == EISDIR)
			exit(126);
		else if (err == ENOENT)
			exit(127);
		exit(1);
	}
}

static void	exec_single_builtin(t_cmd *cmd, t_shell *shell)
{
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (handle_redirections(cmd))
	{
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
		shell->exit_code = 1;
		return ;
	}
	shell->exit_code = exec_builtin(cmd, shell);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}

// data[0] = index (i) | data[1] = total commandes (count)
static void	run_child(t_cmd *cur, t_shell *sh, int *data, int **pipes)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	setup_child_pipes(data[0], data[1], pipes);
	if (handle_redirections(cur))
		exit(1);
	if (!cur->args || !cur->args[0])
		exit(0);
	if (is_builtin(cur->args[0]))
		exit(exec_builtin(cur, sh));
	handle_cmd_execution(cur, sh);
}

static void	exec_multiple_cmds(t_cmd *cmd, t_shell *shell, int count)
{
	int		**pipes;
	pid_t	*pids;
	int		data[2];

	pipes = allocate_pipes(count);
	pids = malloc(sizeof(pid_t) * count);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	data[1] = count;
	data[0] = 0;
	while (cmd)
	{
		pids[data[0]] = fork();
		if (pids[data[0]] == 0)
			run_child(cmd, shell, data, pipes);
		cmd = cmd->next;
		data[0]++;
	}
	close_and_free_pipes(pipes, count);
	wait_children(pids, count, shell);
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}

void	exec_simple_cmd(t_cmd *cmd, t_shell *shell)
{
	int	count;

	if (!cmd)
		return ;
	count = count_cmds(cmd);
	if (count == 1 && cmd->args && cmd->args[0] && is_builtin(cmd->args[0]))
		exec_single_builtin(cmd, shell);
	else
		exec_multiple_cmds(cmd, shell, count);
}
