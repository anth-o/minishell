/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adumaine <adumaine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 11:59:29 by adumaine          #+#    #+#             */
/*   Updated: 2026/02/25 18:14:32 by adumaine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*handle_unclosed_quotes(char *input)
{
	char	*more;
	char	*tmp;
	char	*joined;

	while (check_unclosed_quote(input))
	{
		more = readline("> ");
		if (!more)
			break ;
		tmp = ft_strjoin(input, "\n");
		joined = ft_strjoin(tmp, more);
		free(input);
		free(tmp);
		free(more);
		input = joined;
	}
	return (input);
}

static void	safe_execution(t_cmd *cmds, t_shell *shell)
{
	if (g_signal == SIGINT)
	{
		shell->exit_code = 130;
		g_signal = 0;
	}
	else
		exec_simple_cmd(cmds, shell);
}

static void	process_input(char *input, t_shell *shell)
{
	t_token	*tokens;
	t_cmd	*cmds;

	tokens = argv_to_token(input);
	if (!tokens)
		return ;
	if (check_syntax(tokens))
	{
		shell->exit_code = 2;
		free_tokens(tokens);
		return ;
	}
	expand_vars_tokens(tokens, shell);
	remove_quotes_tokens(tokens);
	cmds = token_to_cmd(tokens);
	if (cmds)
	{
		safe_execution(cmds, shell);
		free_cmds(cmds);
	}
	free_tokens(tokens);
}

static void	minishell_loop(t_shell *shell)
{
	char	*input;

	while (1)
	{
		g_signal = 0;
		input = readline("minishell$ ");
		if (g_signal == SIGINT)
			shell->exit_code = 130;
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		input = handle_unclosed_quotes(input);
		if (input && *input)
		{
			add_history(input);
			process_input(input, shell);
		}
		free(input);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	shell.exit_code = 0;
	shell.env = init_env(envp);
	if (!shell.env && *envp)
		return (1);
	increment_shlvl(shell.env);
	handle_signals();
	minishell_loop(&shell);
	free_env(shell.env);
	rl_clear_history();
	return (shell.exit_code);
}
