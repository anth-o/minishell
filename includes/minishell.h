/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antho <antho@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 12:01:39 by antho             #+#    #+#             */
/*   Updated: 2026/02/15 18:57:59 by antho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

extern int			g_signal;

// LIB

# include "../libft/libft.h"
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

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

typedef struct s_shell
{
	t_env			*env;
	int				exit_code;
}					t_shell;

t_token				*argv_to_token(char *line);
t_cmd				*token_to_cmd(t_token *tokens);
void				remove_quotes_tokens(t_token *head);
void				expand_vars_tokens(t_token *head, t_env *env,
						int exit_code);

int					check_syntax(t_token *tokens);
t_env				*init_env(char **envp);
void				free_env(t_env *env);

char				**env_list_to_tab(t_env *env);
void				free_tab(char **tab);

char				*find_path(char *cmd, t_env *env);
void				exec_simple_cmd(t_cmd *cmd, t_shell *shell);
int					handle_redirections(t_cmd *cmd);

void				update_env(t_env *env, const char *key, const char *value);
char				*get_env_value(t_env *env, char *key);

// Builtins
int					ft_cd(t_shell *shell, t_cmd *cmd);
int					ft_env(t_env *env);
int					ft_exit(t_shell *shell, t_cmd *cmd);
int					ft_pwd(void);
int					is_builtin(char *cmd);
int					exec_builtin(t_cmd *cmd, t_shell *shell);
void				free_cmds(t_cmd *cmds);
void				free_shell(t_shell *shell);
#endif