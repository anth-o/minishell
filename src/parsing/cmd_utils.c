/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antho <antho@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 22:35:28 by antho             #+#    #+#             */
/*   Updated: 2026/02/23 22:35:36 by antho            ###   ########.fr       */
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

void	push_cmd(t_cmd **head, t_cmd **last, t_cmd *curr)
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
		{
			if (cmds->heredoc)
				unlink(cmds->infile); // 🗑️ Nettoyage du fichier temp Heredoc !
			free(cmds->infile);
		}
		if (cmds->outfile)
			free(cmds->outfile);
		free(cmds);
		cmds = tmp;
	}
}

void	add_arg(t_cmd *cmd, char *value)
{
	int		count;
	char	**new_args;
	int		i;

	count = 0;
	if (cmd->args)
		while (cmd->args[count])
			count++;
	new_args = malloc(sizeof(char *) * (count + 2));
	if (!new_args)
		return ;
	i = -1;
	while (++i < count)
		new_args[i] = cmd->args[i];
	new_args[count] = ft_strdup(value);
	new_args[count + 1] = NULL;
	free(cmd->args);
	cmd->args = new_args;
}
