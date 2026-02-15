/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antho <antho@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 11:59:29 by antho             #+#    #+#             */
/*   Updated: 2026/02/15 22:28:10 by antho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_tokens(t_token *head)
{
	t_token	*tmp;

	while (head)
	{
		tmp = head->next;
		free(head->value);
		free(head);
		head = tmp;
	}
}

void	print_tokens(t_token *head)
{
	t_token	*tmp;

	tmp = head;
	while (tmp)
	{
		printf("[%s] (type=%d)\n", tmp->value, tmp->type);
		tmp = tmp->next;
	}
}

void	print_cmds(t_cmd *cmd)
{
	int	i;
	int	j;

	i = 1;
	while (cmd)
	{
		printf("CMD %d:\n", i++);
		j = 0;
		while (cmd->args && cmd->args[j])
		{
			printf("  arg[%d]: %s\n", j, cmd->args[j]);
			j++;
		}
		if (cmd->infile)
			printf("  infile: %s\n", cmd->infile);
		if (cmd->outfile)
		{
			if (cmd->append)
				printf("  outfile: %s (append)\n", cmd->outfile);
			else
				printf("  outfile: %s (truncate)\n", cmd->outfile);
		}
		if (cmd->heredoc)
			printf("  heredoc: yes\n");
		cmd = cmd->next;
	}
}

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_token	*tokens;
	t_cmd	*cmds;
	t_shell	shell;

	(void)argc;
	(void)argv;
	shell.exit_code = 0;
	shell.env = init_env(envp);
	if (!shell.env && *envp)
		return (1);
	handle_signals();
	while (1)
	{
		g_signal = 0;
		input = readline("road_to_BH> ");
		if (g_signal == SIGINT)
			shell.exit_code = 130;
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (*input)
			add_history(input);
		tokens = argv_to_token(input);
		if (tokens)
		{
			if (check_syntax(tokens))
			{
				shell.exit_code = 2;
				free_tokens(tokens);
				free(input);
				continue ;
			}
			expand_vars_tokens(tokens, shell.env, shell.exit_code);
			remove_quotes_tokens(tokens);
			cmds = token_to_cmd(tokens);
			print_cmds(cmds);
			if (cmds)
				exec_simple_cmd(cmds, &shell);
			free_cmds(cmds);
			free_tokens(tokens);
		}
		free(input);
	}
	free_env(shell.env);
	rl_clear_history();
	return (shell.exit_code);
}

/*
BUILTIN
echo - export - unset

HEREDOC <<

SIGNAUX SPECIFIQUES

PIPES |
*/