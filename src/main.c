/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antho <antho@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 11:59:29 by antho             #+#    #+#             */
/*   Updated: 2025/11/24 11:50:59 by antho            ###   ########.fr       */
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

void	free_cmds(t_cmd *cmds)
{
	t_cmd	*tmp;
	int		i;

	while (cmds)
	{
		tmp = cmds->next;
		if (cmds->args)
		{
			i = 0;
			while (cmds->args[i])
			{
				free(cmds->args[i]);
				i++;
			}
			free(cmds->args);
		}
		free(cmds->infile);
		free(cmds->outfile);
		free(cmds);
		cmds = tmp;
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

	(void)argc;
	(void)argv;
	handle_signals();
	while (1)
	{
		input = readline("road_to_BH> ");
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
			expand_vars_tokens(tokens, envp);
			remove_quotes_tokens(tokens);
			cmds = token_to_cmd(tokens);
			print_cmds(cmds);
			free_cmds(cmds);
			free_tokens(tokens);
		}
		free(input);
	}
	return (0);
}
