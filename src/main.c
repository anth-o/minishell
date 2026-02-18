/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ko-mahon <ko-mahon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 11:59:29 by antho             #+#    #+#             */
/*   Updated: 2026/02/18 20:12:11 by ko-mahon         ###   ########.fr       */
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

// void	print_cmds(t_cmd *cmd)
// {
// 	int	i;
// 	int	j;

// 	i = 1;
// 	while (cmd)
// 	{
// 		printf("CMD %d:\n", i++);
// 		j = 0;
// 		while (cmd->args && cmd->args[j])
// 		{
// 			printf("  arg[%d]: %s\n", j, cmd->args[j]);
// 			j++;
// 		}
// 		if (cmd->infile)
// 			printf("  infile: %s\n", cmd->infile);
// 		if (cmd->outfile)
// 		{
// 			if (cmd->append)
// 				printf("  outfile: %s (append)\n", cmd->outfile);
// 			else
// 				printf("  outfile: %s (truncate)\n", cmd->outfile);
// 		}
// 		if (cmd->heredoc)
// 			printf("  heredoc: yes\n");
// 		cmd = cmd->next;
// 	}
// }

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
		input = readline("$ ");
		if (g_signal == SIGINT)
			shell.exit_code = 130;
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		/* If quotes are not closed, read continuation lines with secondary prompt "> "
		   and append them (with a newline) until quotes are balanced. */
		if (input)
		{
			int	in_quote;
			char	q;
			/* check for unclosed quote */
			in_quote = 0;
			q = '\0';
			for (int i = 0; input[i]; i++)
			{
				if (!in_quote && (input[i] == '\'' || input[i] == '"'))
				{
					in_quote = 1;
					q = input[i];
				}
				else if (in_quote && input[i] == q)
				{
					in_quote = 0;
					q = '\0';
				}
			}
			while (in_quote)
			{
				char *more = readline("> ");
				if (!more)
					break ;
				/* append newline + more */
				char *joined = malloc(strlen(input) + strlen(more) + 2);
				if (!joined)
				{
					free(more);
					break ;
				}
				strcpy(joined, input);
				strcat(joined, "\n");
				strcat(joined, more);
				free(input);
				free(more);
				input = joined;
				/* re-evaluate quote state on the new combined input */
				in_quote = 0;
				q = '\0';
				for (int i = 0; input[i]; i++)
				{
					if (!in_quote && (input[i] == '\'' || input[i] == '"'))
					{
						in_quote = 1;
						q = input[i];
					}
					else if (in_quote && input[i] == q)
					{
						in_quote = 0;
						q = '\0';
					}
				}
			}
			if (*input)
				add_history(input);
		}
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
			// print_cmds(cmds);
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