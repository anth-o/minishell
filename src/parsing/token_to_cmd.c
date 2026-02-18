/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_to_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ko-mahon <ko-mahon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 20:36:12 by antho             #+#    #+#             */
/*   Updated: 2026/02/18 20:12:10 by ko-mahon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_cmd	*new_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->append = 0;
	cmd->heredoc = 0;
	cmd->next = NULL;
	return (cmd);
}

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
	new = malloc(strlen(str) + 1);
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

void	add_arg(t_cmd *cmd, char *value)
{
	int		count;
	char	**new_args;
	int		i;

	i = 0;
	count = 0;
	if (cmd->args)
		while (cmd->args[count])
			count++;
	new_args = malloc(sizeof(char *) * (count + 2));
	if (!new_args)
		return ;
	while (i < count)
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	new_args[count] = strdup(value);
	new_args[count + 1] = NULL;
	free(cmd->args);
	cmd->args = new_args;
}

int	assign_redir(t_cmd *curr, t_token **tok)
{
	t_token	*t;

	t = *tok;
	if ((t->type == REDIR_IN || t->type == REDIR_OUT || t->type == REDIR_APPEND
			|| t->type == HEREDOC) && t->next)
	{
		if (t->type == REDIR_IN)
		{
			if (curr->infile)
				free(curr->infile);
			curr->infile = strdup(t->next->value);
		}
		else if (t->type == REDIR_OUT || t->type == REDIR_APPEND)
		{
			/* single outfile behavior: keep last redirection seen */
			if (curr->outfile)
				free(curr->outfile);
			curr->outfile = strdup(t->next->value);
			curr->append = (t->type == REDIR_APPEND) ? 1 : 0;
		}
		else if (t->type == HEREDOC)
		{
			if (curr->infile)
				free(curr->infile);
			curr->infile = strdup(t->next->value);
			curr->heredoc = 1;
		}
		*tok = t->next;
		return (1);
	}
	return (0);
}

static void	push_cmd(t_cmd **head, t_cmd **last, t_cmd *curr)
{
	if (!*head)
		*head = curr;
	else
		(*last)->next = curr;
	*last = curr;
}

int	is_redir(t_token *t)
{
	return (t->type == REDIR_IN || t->type == REDIR_OUT
		|| t->type == REDIR_APPEND || t->type == HEREDOC);
}

void	free_cmds(t_cmd *cmds)
{
	t_cmd	*tmp;

	while (cmds)
	{
		tmp = cmds->next;
		if (cmds->args)
			free_tab(cmds->args);
		if (cmds->infile)
			free(cmds->infile);
		if (cmds->outfile)
			free(cmds->outfile);
		free(cmds);
		cmds = tmp;
	}
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
