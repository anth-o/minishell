/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antho <antho@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 22:49:13 by antho             #+#    #+#             */
/*   Updated: 2026/02/24 00:45:39 by antho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	read_heredoc(int fd, char *limiter)
{
	char	*line;

	// L'enfant accepte de se faire tuer par Ctrl+C
	signal(SIGINT, SIG_DFL);
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
	int fd;
	char *tmp_name;
	pid_t pid;
	int status;

	tmp_name = ft_strdup(".heredoc_tmp");
	if (!tmp_name)
		return (NULL);
	fd = open(tmp_name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
	{
		free(tmp_name);
		return (NULL);
	}
	pid = fork();
	if (pid == 0)
		read_heredoc(fd, limiter);
	close(fd);
	waitpid(pid, &status, 0);
	// Si l'enfant a été tué par Ctrl+C, on prévient le shell !
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		g_signal = SIGINT;
	return (tmp_name);
}