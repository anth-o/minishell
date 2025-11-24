/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antho <antho@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 12:01:39 by antho             #+#    #+#             */
/*   Updated: 2025/11/18 18:30:44 by antho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

extern int			g_last_exit;

// LIB

# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

void				handle_signals(void);
void				handle_sigint(int sig);

typedef enum e_tokentype
{
	WORD,
	SINGLE_QUOTE,
	DOUBLE_QUOTE,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	HEREDOC
}					t_tokentype;

typedef struct s_token
{
	char			*value;
	t_tokentype		type;
	struct s_token	*next;
}					t_token;

typedef struct s_cmd
{
	char **args;        // ex : {"ls", "-l", NULL}
	char *infile;       // pour < redirection
	char *outfile;      // pour > redirection
	int append;         // 1 si >>
	int heredoc;        // 1 si <<
	struct s_cmd *next; // commande suivante si pipe
}					t_cmd;

t_token				*argv_to_token(char *line);
t_cmd				*token_to_cmd(t_token *tokens);
void				remove_quotes_tokens(t_token *head);
void				expand_vars_tokens(t_token *head, char **envp);

#endif