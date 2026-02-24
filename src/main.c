/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antho <antho@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 11:59:29 by adumaine          #+#    #+#             */
/*   Updated: 2026/02/24 00:57:43 by antho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	check_unclosed_quote(char *str)
{
	int		i;
	int		in_quote;
	char	q;

	i = -1;
	in_quote = 0;
	q = '\0';
	while (str[++i])
	{
		if (!in_quote && (str[i] == '\'' || str[i] == '"'))
		{
			in_quote = 1;
			q = str[i];
		}
		else if (in_quote && str[i] == q)
		{
			in_quote = 0;
			q = '\0';
		}
	}
	return (in_quote);
}

static char	*handle_unclosed_quotes(char *input)
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
		// MAGIE ICI : On n'exécute QUE si on n'a pas fait Ctrl+C dans le Heredoc
		if (g_signal != SIGINT)
			exec_simple_cmd(cmds, shell);
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
