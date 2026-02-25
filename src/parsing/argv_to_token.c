/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argv_to_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adumaine <adumaine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 15:12:16 by adumaine          #+#    #+#             */
/*   Updated: 2026/02/25 18:26:07 by adumaine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	handle_operator(char *line, int *i, t_token **h, t_token **l)
{
	if (line[*i] == '|')
		add_token(h, l, new_token(line + *i, 1, PIPE));
	else if (line[*i] == '<' && line[*i + 1] == '<')
		add_token(h, l, new_token(line + *i, 2, HEREDOC));
	else if (line[*i] == '<')
		add_token(h, l, new_token(line + *i, 1, REDIR_IN));
	else if (line[*i] == '>' && line[*i + 1] == '>')
		add_token(h, l, new_token(line + *i, 2, REDIR_APPEND));
	else if (line[*i] == '>')
		add_token(h, l, new_token(line + *i, 1, REDIR_OUT));
	else
		return (0);
	if ((line[*i] == '<' && line[*i + 1] == '<') || (line[*i] == '>' && line[*i
				+ 1] == '>'))
		*i += 2;
	else
		(*i)++;
	return (1);
}

static void	handle_word(char *line, int *i, t_token **h, t_token **l)
{
	int	start;

	start = *i;
	while (line[*i] && !is_delimiter(line[*i]))
	{
		if (line[*i] == '\'' || line[*i] == '"')
			*i = skip_quotes(line, *i);
		else
			(*i)++;
	}
	add_token(h, l, new_token(line + start, *i - start, WORD));
}

t_token	*argv_to_token(char *line)
{
	t_token	*head;
	t_token	*last;
	int		i;

	head = NULL;
	last = NULL;
	i = 0;
	while (line[i])
	{
		skip_spaces(line, &i);
		if (!line[i])
			break ;
		if (handle_operator(line, &i, &head, &last))
			continue ;
		handle_word(line, &i, &head, &last);
	}
	return (head);
}
