/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antho <antho@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 22:49:13 by antho             #+#    #+#             */
/*   Updated: 2026/02/24 22:41:19 by antho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	heredoc_sigint(int sig)
{
	(void)sig;
	/* Le terminal affiche déjà le ^C,
		donc on a juste besoin de sauter la ligne ! */
	ft_putstr_fd("\n", 1);
	exit(130);
}

static void	read_heredoc(int fd, char *limiter)
{
	char	*line;

	/* 2. On branche notre handler custom */
	signal(SIGINT, heredoc_sigint);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, limiter) == 0)
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, fd);
		ft_putstr_fd("\n", fd);
		free(line);
	}
	close(fd);
	exit(0);
}

char	*ft_heredoc(char *limiter)
{
	int		fd;
	char	*tmp;
	pid_t	pid;
	int		status;

	tmp = ft_strdup(".heredoc_tmp");
	if (!tmp)
		return (NULL);
	fd = open(tmp, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		return (free(tmp), NULL);
	pid = fork();
	if (pid == 0)
		read_heredoc(fd, limiter);
	close(fd);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	waitpid(pid, &status, 0);
	signal(SIGINT, handle_sigint);
	/* 3. On détecte que l'enfant a quitté via notre handler (code 130) */
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
		g_signal = SIGINT;
	/* On a enlevé le printf("\n") d'avant, car l'enfant l'a déjà fait ! */
	return (tmp);
}
