/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ko-mahon <ko-mahon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 17:57:56 by antho             #+#    #+#             */
/*   Updated: 2026/02/18 20:12:08 by ko-mahon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

// Fonctions append_char et append_str inchangées...
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
		ft_strlcat(new, str, len + 2);
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
		ft_strlcat(new, dest, total_len);
		free(dest);
	}
	if (src)
		ft_strlcat(new, src, total_len);
	return (new);
}

// MODIFICATION 2: Ajout du paramètre exit_code et t_env
char	*expand_var(const char *var_start, t_env *env, int *consumed_len,
		int exit_code)
{
	int		len;
	char	*name;
	char	*value;
	char	*expanded;

	// Gestion du $?
	if (var_start[0] == '?')
	{
		*consumed_len = 1;
		return (ft_itoa(exit_code));
	}
	len = var_name_len(var_start);
	name = strndup(var_start, len);
	// CORRECTION ICI :
	// 1. On inverse les arguments : (env, name) au lieu de (name, env)
	value = get_env_value(env, name);
	// 2. On gère le cas NULL (si la variable n'existe pas, on renvoie vide)
	if (value)
		expanded = strdup(value);
	else
		expanded = strdup(""); // Important pour echo $INEXISTANT
	free(name);
	*consumed_len = len;
	return (expanded);
}

// MODIFICATION 3: Propagation des paramètres
static void	process_expansion(char **res, const char *input, int *i, t_env *env,
		int exit_code)
{
	int		consumed;
	char	*exp;

	(*i)++;
	exp = expand_var(input + *i, env, &consumed, exit_code);
	*res = append_str(*res, exp);
	free(exp);
	*i += consumed;
}

// MODIFICATION 4: Propagation des paramètres
char	*expand_variables(const char *input, t_env *env, int exit_code)
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
		{
			/* Only expand if there's a valid variable name after $ */
			int vlen = var_name_len(input + i + 1);
			if (vlen > 0 || input[i + 1] == '?')
				process_expansion(&res, input, &i, env, exit_code);
			else
				/* treat lone $ as literal */
				res = append_char(res, input[i++]);
		}
		else
			res = append_char(res, input[i++]);
	}
	return (res);
}

// MODIFICATION 5: Fonction principale appelée par le main
void	expand_vars_tokens(t_token *head, t_env *env, int exit_code)
{
	t_token	*tmp;
	char	*new_val;

	tmp = head;
	while (tmp)
	{
		if (tmp->type == WORD)
		{
			new_val = expand_variables(tmp->value, env, exit_code);
			free(tmp->value);
			tmp->value = new_val;
		}
		tmp = tmp->next;
	}
}
