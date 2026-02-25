/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adumaine <adumaine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 12:01:39 by adumaine          #+#    #+#             */
/*   Updated: 2026/02/25 18:27:52 by adumaine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

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

/* Global variable for signals */
extern int			g_signal;

/* --- Structures --- */

typedef enum e_tokentype
{
	WORD,
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
	char			**args;
	char			*infile;
	char			*outfile;
	int				append;
	int				heredoc;
	struct s_cmd	*next;
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

/* --- Prototypes --- */

/* Main & Signals */
void				handle_signals(void);
void				handle_sigint(int sig);

/* Parsing - Lexer (argv_to_token.c & token_utils.c) */
t_token				*argv_to_token(char *line);
t_token				*new_token(char *start, int len, t_tokentype type);
void				add_token(t_token **head, t_token **last, t_token *new);
int					skip_quotes(char *line, int i);
void				skip_spaces(char *line, int *i);
int					is_delimiter(char c);

/* Parsing - Expansion & Quotes (handle_vars.c & quotes_utils.c) */
void				expand_vars_tokens(t_token *head, t_shell *shell);
char				*expand_var(const char *var_start, t_env *env,
						int *consumed_len, int exit_code);
void				remove_quotes_tokens(t_token *head);
char				*remove_quotes(const char *str);
int					var_name_len(const char *s);
char				*append_char(char *str, char c);
char				*append_str(char *dest, char *src);

/* Parsing - Commands (token_to_cmd.c & cmd_utils.c) */
t_cmd				*token_to_cmd(t_token *tokens);
t_cmd				*new_cmd(void);
void				push_cmd(t_cmd **head, t_cmd **last, t_cmd *curr);
void				add_arg(t_cmd *cmd, char *value);
void				free_cmds(t_cmd *cmds);
int					is_redir(t_token *t);
int					check_syntax(t_token *tokens);
int					check_unclosed_quote(char *str);
void				free_tokens(t_token *head);

/* Execution Core */
void				exec_simple_cmd(t_cmd *cmd, t_shell *shell);
char				*find_path(char *cmd, t_env *env);
int					handle_redirections(t_cmd *cmd);
char				*ft_heredoc(char *limiter);

/* Execution Utils */
int					count_cmds(t_cmd *cmd);
int					**allocate_pipes(int count);
void				setup_child_pipes(int i, int count, int **pipes);
void				close_and_free_pipes(int **pipes, int count);
void				wait_children(pid_t *pids, int count, t_shell *shell);

/* Env Management (env_init.c & env_utils.c) */
t_env				*init_env(char **envp);
void				update_env(t_env *env, const char *key, const char *value);
char				*get_env_value(t_env *env, char *key);
char				**env_list_to_tab(t_env *env);
void				increment_shlvl(t_env *env);
void				free_env(t_env *env);
void				free_tab(char **tab);

/* Builtins */
int					is_builtin(char *cmd);
int					exec_builtin(t_cmd *cmd, t_shell *shell);
int					ft_echo(t_cmd *cmd);
int					ft_cd(t_shell *shell, t_cmd *cmd);
int					ft_pwd(t_shell *shell);
int					ft_export(t_shell *shell, t_cmd *cmd);
int					ft_unset(t_shell *shell, t_cmd *cmd);
int					ft_env(t_env *env);
int					ft_exit(t_shell *shell, t_cmd *cmd);

/* Builtin Utils (export) */
int					is_valid_key(const char *key);
void				print_export(t_env *env);

#endif