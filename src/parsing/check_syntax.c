/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antho <antho@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 22:32:14 by antho             #+#    #+#             */
/*   Updated: 2026/02/14 13:22:22 by antho            ###   ########.fr       */
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
