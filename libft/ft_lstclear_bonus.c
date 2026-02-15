/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antho <antho@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 13:13:47 by antho             #+#    #+#             */
/*   Updated: 2025/04/13 18:04:45 by antho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*buff;

	if (!lst || !del)
		return ;
	while (*lst)
	{
		buff = (*lst)->next;
		ft_lstdelone(*lst, del);
		*lst = buff;
	}
}
