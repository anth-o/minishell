/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antho <antho@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 17:57:56 by antho             #+#    #+#             */
/*   Updated: 2025/11/24 12:59:42 by antho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			g_last_exit = 0;

static int	get_len(int n)
{
	int	len;

	len = 0;
	if (n <= 0)
		len++;
	while (n)
	{
		n /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char	*str;
	int		len;
	long	nbr;

	nbr = n;
	len = get_len(n);
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	str[len--] = '\0';
	if (nbr == 0)
		str[0] = '0';
	if (nbr < 0)
	{
		str[0] = '-';
		nbr = -nbr;
	}
	while (nbr > 0)
	{
		str[len--] = (nbr % 10) + '0';
		nbr /= 10;
	}
	return (str);
}

int	var_name_len(const char *s)
{
	int	i;

	i = 0;
	if (s[0] == '?' || (s[0] >= '0' && s[0] <= '9'))
		return (1);
	while (s[i] && ((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z')
			|| (s[i] >= '0' && s[i] <= '9') || s[i] == '_'))
		i++;
	return (i);
}

char	*get_env_value(const char *name, char **envp)
{
	int	i;
	int	len;

	i = 0;
	len = strlen(name);
	while (envp[i])
	{
		if (!strncmp(envp[i], name, len) && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return ("");
}

char	*append_char(char *str, char c)
{
	char	*new;
	int		len;

	if (str)
		len = strlen(str);
	else
		len = 0;
	new = calloc(len + 2, sizeof(char));
	if (!new)
		return (NULL);
	if (str)
	{
		strlcat(new, str, len + 2);
		free(str);
	}
	new[len] = c;
	return (new);
}

char	*append_str(char *dest, char *src)
{
	char	*new;
	int		len_dest;
	int		len_src;
	int		total_len;

	if (dest)
		len_dest = strlen(dest);
	else
		len_dest = 0;
	if (src)
		len_src = strlen(src);
	else
		len_src = 0;
	total_len = len_dest + len_src + 1;
	new = calloc(total_len, sizeof(char));
	if (!new)
		return (NULL);
	if (dest)
	{
		strlcat(new, dest, total_len);
		free(dest);
	}
	if (src)
		strlcat(new, src, total_len);
	return (new);
}

char	*expand_var(const char *var_start, char **envp, int *consumed_len)
{
	int		len;
	char	*name;
	char	*value;
	char	*expanded;

	if (var_start[0] == '?')
	{
		*consumed_len = 1;
		return (ft_itoa(g_last_exit));
	}
	len = var_name_len(var_start);
	name = strndup(var_start, len);
	value = get_env_value(name, envp);
	expanded = strdup(value);
	free(name);
	*consumed_len = len;
	return (expanded);
}

static void	process_expansion(char **res, const char *input, int *i, char **env)
{
	int		consumed;
	char	*exp;

	(*i)++;
	exp = expand_var(input + *i, env, &consumed);
	*res = append_str(*res, exp);
	free(exp);
	*i += consumed;
}

char	*expand_variables(const char *input, char **envp)
{
	int		i;
	int		sq;
	int		dq;
	char	*res;

	i = 0;
	sq = 0;
	dq = 0;
	res = calloc(1, sizeof(char));
	if (!res)
		return (NULL);
	while (input[i])
	{
		if (input[i] == '\'' && !dq)
		{
			sq = !sq;
			res = append_char(res, input[i++]);
		}
		else if (input[i] == '"' && !sq)
		{
			dq = !dq;
			res = append_char(res, input[i++]);
		}
		else if (input[i] == '$' && !sq && !(input[i + 1] == '\0' || input[i
				+ 1] == ' ' || (input[i + 1] == '"' && !dq)))
			process_expansion(&res, input, &i, envp);
		else
			res = append_char(res, input[i++]);
	}
	return (res);
}

void	expand_vars_tokens(t_token *head, char **envp)
{
	t_token	*tmp;
	char	*new_val;

	tmp = head;
	while (tmp)
	{
		if (tmp->type == WORD)
		{
			new_val = expand_variables(tmp->value, envp);
			free(tmp->value);
			tmp->value = new_val;
		}
		tmp = tmp->next;
	}
}
