/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antho <antho@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 22:36:49 by antho             #+#    #+#             */
/*   Updated: 2026/02/23 22:36:58 by antho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_quote_closed(const char *str, int i)
{
	char	q;

	q = str[i];
	i++;
	while (str[i])
	{
		if (str[i] == q)
			return (1);
		i++;
	}
	return (0);
}

char	*remove_quotes(const char *str)
{
	int		i;
	int		j;
	char	quote;
	char	*new;

	i = 0;
	j = 0;
	new = malloc(ft_strlen(str) + 1);
	if (!new)
		return (NULL);
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '"') && is_quote_closed(str, i))
		{
			quote = str[i++];
			while (str[i] && str[i] != quote)
				new[j++] = str[i++];
			if (str[i] == quote)
				i++;
		}
		else
			new[j++] = str[i++];
	}
	new[j] = '\0';
	return (new);
}

void	remove_quotes_tokens(t_token *head)
{
	t_token	*tmp;
	char	*new_val;

	tmp = head;
	while (tmp)
	{
		if (tmp->type == WORD)
		{
			new_val = remove_quotes(tmp->value);
			free(tmp->value);
			tmp->value = new_val;
		}
		tmp = tmp->next;
	}
}
