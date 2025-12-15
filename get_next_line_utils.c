/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 18:48:42 by rapohlen          #+#    #+#             */
/*   Updated: 2025/12/15 19:49:06 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	get_len(t_gnl_buf *buf, int end_len)
{
	int	len;

	len = end_len;
	while (buf->next)
	{
		len += buf->len - buf->index;
		buf = buf->next;
	}
	return (len);
}

int	gnl_clear_buf(t_gnl_buf *buf, char **line)
{
	t_gnl_buf	*last;

	while (buf)
	{
		last = buf;
		buf = buf->next;
		free(last);
	}
	*line = NULL;
	return (1);
}

t_gnl	*lst_add_fd(t_gnl **lst, int fd)
{
	t_gnl	*new;

	new = malloc(sizeof(*new));
	if (!new)
		return (NULL);
	new->fd = fd;
	new->buf = NULL;
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
