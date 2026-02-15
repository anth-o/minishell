/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argv_to_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antho <antho@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 15:12:16 by antho             #+#    #+#             */
/*   Updated: 2026/02/14 17:15:25 by antho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_token	*new_token(char *start, int len, t_tokentype type)
{
	t_token	*t;

	t = malloc(sizeof(t_token));
	if (!t)
		return (NULL);
	t->value = strndup(start, len);
	t->type = type;
	t->next = NULL;
	return (t);
}

static void	add_token(t_token **head, t_token **last, t_token *new)
{
	if (!*head)
		*head = new;
	else
		(*last)->next = new;
	*last = new;
}

static int	skip_quotes(char *line, int i)
{
	char	quote;

	quote = line[i];
	i++;
	while (line[i] && line[i] != quote)
		i++;
	if (line[i] == quote)
		i++;
	return (i);
}

static void	skip_spaces(char *line, int *i)
{
	while (line[*i] && (line[*i] == ' ' || line[*i] == '\t'))
		(*i)++;
}

static int	handle_operator(char *line, int *i, t_token **h, t_token **l)
{
	if (line[*i] == '|')
		return (add_token(h, l, new_token(line + (*i)++, 1, PIPE)), 1);
	if (line[*i] == '<')
	{
		if (line[*i + 1] == '<')
			return (add_token(h, l, new_token(line + *i, 2, HEREDOC)), *i += 2,
				1);
		return (add_token(h, l, new_token(line + (*i)++, 1, REDIR_IN)), 1);
	}
	if (line[*i] == '>')
	{
		if (line[*i + 1] == '>')
			return (add_token(h, l, new_token(line + *i, 2, REDIR_APPEND)), *i
				+= 2, 1);
		return (add_token(h, l, new_token(line + (*i)++, 1, REDIR_OUT)), 1);
	}
	return (0);
}

static void	handle_word(char *line, int *i, t_token **h, t_token **l)
{
	int	start;

	start = *i;
	while (line[*i] && line[*i] != ' ' && line[*i] != '\t' && line[*i] != '|'
		&& line[*i] != '<' && line[*i] != '>')
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
