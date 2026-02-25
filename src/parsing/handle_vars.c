/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adumaine <adumaine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 17:57:56 by adumaine          #+#    #+#             */
/*   Updated: 2026/02/25 18:25:27 by adumaine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*expand_var(const char *var_start, t_env *env, int *consumed_len,
		int exit_code)
{
	int		len;
	char	*name;
	char	*value;
	char	*expanded;

	if (var_start[0] == '?')
	{
		*consumed_len = 1;
		return (ft_itoa(exit_code));
	}
	len = var_name_len(var_start);
	name = ft_substr(var_start, 0, len);
	value = get_env_value(env, name);
	if (value)
		expanded = ft_strdup(value);
	else
		expanded = ft_strdup("");
	free(name);
	*consumed_len = len;
	return (expanded);
}

static void	handle_dollar(char **res, const char *input, int *i, t_shell *shell)
{
	int		vlen;
	int		consumed;
	char	*exp;

	vlen = var_name_len(input + *i + 1);
	if (vlen > 0 || input[*i + 1] == '?')
	{
		(*i)++;
		exp = expand_var(input + *i, shell->env, &consumed, shell->exit_code);
		*res = append_str(*res, exp);
		free(exp);
		*i += consumed;
	}
	else
	{
		*res = append_char(*res, input[*i]);
		(*i)++;
	}
}

static void	process_char(char **res, const char *input, int *i, int *st)
{
	if (input[*i] == '\'' && !st[1])
	{
		st[0] = !st[0];
		*res = append_char(*res, input[(*i)++]);
	}
	else if (input[*i] == '"' && !st[0])
	{
		st[1] = !st[1];
		*res = append_char(*res, input[(*i)++]);
	}
	else
		*res = append_char(*res, input[(*i)++]);
}

char	*expand_variables(const char *input, t_shell *shell)
{
	int		i;
	int		st[2];
	char	*res;

	i = 0;
	st[0] = 0;
	st[1] = 0;
	res = ft_calloc(1, sizeof(char));
	if (!res)
		return (NULL);
	while (input[i])
	{
		if (input[i] == '$' && !st[0] && input[i + 1] && input[i + 1] != ' '
			&& !(input[i + 1] == '"' && !st[1]))
			handle_dollar(&res, input, &i, shell);
		else
			process_char(&res, input, &i, st);
	}
	return (res);
}

void	expand_vars_tokens(t_token *head, t_shell *shell)
{
	t_token	*tmp;
	char	*new_val;

	tmp = head;
	while (tmp)
	{
		if (tmp->type == WORD)
		{
			new_val = expand_variables(tmp->value, shell);
			free(tmp->value);
			tmp->value = new_val;
		}
		tmp = tmp->next;
	}
}
