/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adumaine <adumaine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 22:26:03 by adumaine          #+#    #+#             */
/*   Updated: 2026/02/25 18:25:01 by adumaine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*new_token(char *start, int len, t_tokentype type)
{
	t_token	*t;

	t = malloc(sizeof(t_token));
	if (!t)
		return (NULL);
	t->value = ft_substr(start, 0, len);
	t->type = type;
	t->next = NULL;
	return (t);
}

void	add_token(t_token **head, t_token **last, t_token *new)
{
	if (!*head)
		*head = new;
	else
		(*last)->next = new;
	*last = new;
}

int	skip_quotes(char *line, int i)
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

void	skip_spaces(char *line, int *i)
{
	while (line[*i] && (line[*i] == ' ' || line[*i] == '\t'))
		(*i)++;
}

int	is_delimiter(char c)
{
	return (c == ' ' || c == '\t' || c == '|' || c == '<' || c == '>');
}
