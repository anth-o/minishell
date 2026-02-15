/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antho <antho@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 21:05:40 by antho             #+#    #+#             */
/*   Updated: 2026/02/14 21:26:59 by antho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// Gère l'entrée (<)
static int	check_infile(t_cmd *cmd)
{
	int	fd;

	if (!cmd->infile || cmd->heredoc) // On ignore heredoc pour l'instant
		return (0);
	fd = open(cmd->infile, O_RDONLY);
	if (fd < 0)
	{
		// Affiche : "minishell: nom_fichier: No such file or directory"
		perror(cmd->infile);
		return (1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

// Gère la sortie (> et >>)
static int	check_outfile(t_cmd *cmd)
{
	int	fd;
	int	flags;

	if (!cmd->outfile)
		return (0);
	if (cmd->append)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	// 0644 = Droits classiques (Lecture/Ecriture pour toi,
	fd = open(cmd->outfile, flags, 0644);
	if (fd < 0)
	{
		perror(cmd->outfile);
		return (1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

// Fonction principale
int	handle_redirections(t_cmd *cmd)
{
	if (check_infile(cmd))
		return (1);
	if (check_outfile(cmd))
		return (1);
	return (0);
}
