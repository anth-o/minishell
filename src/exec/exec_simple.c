/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_simple.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ko-mahon <ko-mahon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 19:20:58 by antho             #+#    #+#             */
/*   Updated: 2026/02/18 20:12:08 by ko-mahon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <sys/wait.h> // Pour waitpid

void	handle_cmd_execution(t_cmd *cmd, t_shell *shell)
{
	char	*path;
	char	**env_tab;

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
		int err = errno;
		if (err == EACCES || err == EISDIR)
			exit(126);
		else if (err == ENOENT)
			exit(127);
		else
			exit(1);
	}
}

/*
 * Execute a list of commands possibly connected by pipes.
 * Strategy: fork every command (including builtins), connect pipes,
 * apply redirections in the child, then execute builtin or external.
 */
void	exec_simple_cmd(t_cmd *cmd, t_shell *shell)
{
	int		cmd_count;
	t_cmd	*cur;
	int		i;
	int		**pipes;
	pid_t	*pids;
	pid_t	pid;
	int		status;

	if (!cmd)
		return ;
	/* Count commands */
	cmd_count = 0;
	cur = cmd;
	while (cur)
	{
		cmd_count++;
		cur = cur->next;
	}
	/* Allocate pipes (cmd_count - 1) */
	pipes = NULL;
	if (cmd_count > 1)
	{
		pipes = malloc(sizeof(int *) * (cmd_count - 1));
		if (!pipes)
			return ;
		i = 0;
		while (i < cmd_count - 1)
		{
			pipes[i] = malloc(sizeof(int) * 2);
			if (!pipes[i] || pipe(pipes[i]) == -1)
			{
				perror("pipe");
				return ;
			}
			i++;
		}
	}
	pids = malloc(sizeof(pid_t) * cmd_count);
	if (!pids)
		return ;

	/* If single builtin (no pipes), execute it in the parent so `exit` exits the shell */
	if (cmd_count == 1 && is_builtin(cmd->args[0]))
	{
		int saved_stdin = dup(STDIN_FILENO);
		int saved_stdout = dup(STDOUT_FILENO);
		if (handle_redirections(cmd))
		{
			/* restore fds */
			dup2(saved_stdin, STDIN_FILENO);
			dup2(saved_stdout, STDOUT_FILENO);
			close(saved_stdin);
			close(saved_stdout);
			shell->exit_code = 1;
			return ;
		}
		/* execute builtin directly in parent */
		shell->exit_code = exec_builtin(cmd, shell);
		/* restore std fds */
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
		return ;
	}

	/* Parent: ignore signals while children run to avoid printing extra prompts */
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);

	/* Fork each command */
	i = 0;
	cur = cmd;
	while (cur)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			/* cleanup omitted for brevity */
			return ;
		}
		if (pid == 0)
		{
			/* In child, restore default signal handling so Ctrl-C/Ctrl-\ affect the child */
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			/* Child: set up pipes */
			if (cmd_count > 1)
			{
				/* If not first, redirect stdin to read end of previous pipe */
				if (i > 0)
				{
					if (dup2(pipes[i - 1][0], STDIN_FILENO) == -1)
						exit(1);
				}
				/* If not last, redirect stdout to write end of current pipe */
				if (i < cmd_count - 1)
				{
					if (dup2(pipes[i][1], STDOUT_FILENO) == -1)
						exit(1);
				}
				/* Close all pipe fds in child */
				for (int j = 0; j < cmd_count - 1; j++)
				{
					close(pipes[j][0]);
					close(pipes[j][1]);
				}
			}
			/* Apply redirections (infile/outfile). This will override pipe fds if needed */
			if (handle_redirections(cur))
				exit(1);

			/* Execute builtin in child or external */
			if (is_builtin(cur->args[0]))
			{
				int ret = exec_builtin(cur, shell);
				exit(ret);
			}
			handle_cmd_execution(cur, shell);
		}
		else
		{
			/* Parent: store pid and continue */
			pids[i] = pid;
			i++;
			cur = cur->next;
		}
	}

	/* Parent: close all pipe fds */
	if (cmd_count > 1)
	{
		for (int j = 0; j < cmd_count - 1; j++)
		{
			close(pipes[j][0]);
			close(pipes[j][1]);
			free(pipes[j]);
		}
		free(pipes);
	}

	/* Wait for all children and set exit code to last child's status */
	for (i = 0; i < cmd_count; i++)
	{
		waitpid(pids[i], &status, 0);
		if (i == cmd_count - 1)
		{
			if (WIFEXITED(status))
				shell->exit_code = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			{
				int sig = WTERMSIG(status);
				/* If terminated by SIGQUIT, print message similar to bash */
				if (sig == SIGQUIT)
				{
					if (WCOREDUMP(status))
						ft_putstr_fd("Quit (core dumped)\n", 2);
					else
						ft_putstr_fd("Quit\n", 2);
				}
				/* If terminated by SIGINT, emit a newline so the prompt appears on next line */
				else if (sig == SIGINT)
				{
					write(1, "\n", 1);
				}
				shell->exit_code = 128 + sig;
			}
		}
	}
	free(pids);

	/* Restore parent signal handlers */
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}
