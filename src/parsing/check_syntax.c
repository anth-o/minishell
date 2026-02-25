/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adumaine <adumaine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 22:32:14 by adumaine          #+#    #+#             */
/*   Updated: 2026/02/25 18:26:03 by adumaine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	print_syntax_error(char *token)
{
	printf("minishell: syntax error near unexpected token '%s'\n", token);
	return (1);
}

int	check_syntax(t_token *tokens)
{
	if (!tokens)
		return (0);
	if (tokens->type == PIPE)
		return (print_syntax_error("|"));
	while (tokens)
	{
		if (tokens->type == PIPE)
		{
			if (!tokens->next || tokens->next->type == PIPE)
				return (print_syntax_error("|"));
		}
		else if (tokens->type == REDIR_IN || tokens->type == REDIR_OUT
			|| tokens->type == REDIR_APPEND || tokens->type == HEREDOC)
		{
			if (!tokens->next)
				return (print_syntax_error("newline"));
			if (tokens->next->type != WORD)
				return (print_syntax_error(tokens->next->value));
		}
		tokens = tokens->next;
	}
	return (0);
}

int	check_unclosed_quote(char *str)
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
