/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 17:29:16 by rapohlen          #+#    #+#             */
/*   Updated: 2025/10/11 12:51:17 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

// + 1 malloc is handled directly in realloc for simplicity
char	*ft_realloc(char *old, size_t old_size, size_t to_add)
{
	char	*new;
	size_t	i;

	new = malloc(old_size + to_add + 1);
	if (!new)
	{
		free(old);
		return (new);
	}
	i = 0;
	while (i < old_size)
	{
		new[i] = old[i];
		i++;
	}
	free(old);
	return (new);
}

// always init index and len values since we will attempt a copy before 1st read
t_gnl	*lst_add_fd(t_gnl **lst, int fd)
{
	t_gnl	*new;

	new = malloc(sizeof(*new));
	if (!new)
		return (new);
	new->fd = fd;
	new->index = 0;
	new->len = 0;
	new->next = NULL;
	if (!*lst)
		*lst = new;
	else
	{
		new->next = *lst;
		*lst = new;
	}
	return (new);
}

void	lst_remove_fd(t_gnl **lst, int fd)
{
	t_gnl	*last;
	t_gnl	*to_remove;

	to_remove = *lst;
	while (to_remove && to_remove->fd != fd)
	{
		last = to_remove;
		to_remove = to_remove->next;
	}
	if (to_remove)
	{
		if (to_remove == *lst)
			*lst = to_remove->next;
		else
			last->next = to_remove->next;
		free(to_remove);
	}
}

t_gnl	*lst_find_fd(t_gnl *lst, int fd)
{
	while (lst)
	{
		if (lst->fd == fd)
			return (lst);
		lst = lst->next;
	}
	return (NULL);
}
