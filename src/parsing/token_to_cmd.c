/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_to_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antho <antho@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 20:36:12 by antho             #+#    #+#             */
/*   Updated: 2026/02/24 21:33:56 by antho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

static void	set_infile(t_cmd *cur, t_token *t)
{
	if (cur->infile)
		free(cur->infile);
	if (t->type == HEREDOC)
	{
		cur->infile = ft_heredoc(t->next->value); // 🚀 Connexion du Heredoc
		cur->heredoc = 1;
	}
	else
		cur->infile = ft_strdup(t->next->value);
}

int	assign_redir(t_cmd *cur, t_token **tok)
{
	t_token	*t;
	int		fd;

	t = *tok;
	if (!is_redir(t) || !t->next)
		return (0);
	if (t->type == REDIR_IN || t->type == HEREDOC)
		set_infile(cur, t);
	else
	{
		/* --- NOUVEAUTÉ : On crée le fichier physiquement --- */
		if (t->type == REDIR_OUT)
			fd = open(t->next->value, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		else
			fd = open(t->next->value, O_CREAT | O_WRONLY | O_APPEND, 0644);
		if (fd >= 0)
			close(fd);
		/* --------------------------------------------------- */
		if (cur->outfile)
			free(cur->outfile);
		cur->outfile = ft_strdup(t->next->value);
		cur->append = (t->type == REDIR_APPEND);
	}
	*tok = t->next;
	return (1);
}

t_cmd	*token_to_cmd(t_token *tok)
{
	t_cmd	*head;
	t_cmd	*last;
	t_cmd	*curr;

	head = NULL;
	last = NULL;
	curr = new_cmd();
	while (tok)
	{
		if (tok->type == WORD)
			add_arg(curr, tok->value);
		else if (tok->type == PIPE)
		{
			push_cmd(&head, &last, curr);
			curr = new_cmd();
		}
		else if (is_redir(tok))
			assign_redir(curr, &tok);
		tok = tok->next;
	}
	push_cmd(&head, &last, curr);
	return (head);
}
